#include "flow_apply_u_ticket.hpp"

using namespace std;
using json = nlohmann::json;

void FlowApplyUTicket::holder_apply_u_ticket(string device_id, string cmd) {
    // [STAGE: (VL)(L)]
    string stored_u_ticket_json = shared_data->device_table[device_id].device_u_ticket_for_owner;

    // [STAGE: (VR)]
    UTicket stored_u_ticket = msg_verifier->self_classify_u_ticket_is_defined_type(stored_u_ticket_json);

    if (stored_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET
        || stored_u_ticket.u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET) \
    {
        // [STAGE: (C)]
        executor->self_change_state(this_device::STATE_AGENT_WAIT_FOR_RT);
    } else if (stored_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_UTICKET
    || stored_u_ticket.u_ticket_type == u_ticket::TYPE_SELFACCESS_UTICKET) {
        // [STAGE: (E)]
        RTicket empty_r_ticket;
        executor->self_execute_cr_ke(1, stored_u_ticket, empty_r_ticket, "holder", cmd);

        // [STAGE: (C)]
        executor->self_change_state(this_device::STATE_AGENT_WAIT_FOR_CRKE1);
    }

    // [STAGE: (S)]
    msg_sender->send_xxx_message(
        message::MESSAGE_VERIFY_AND_EXECUTE,
        u_ticket::MESSAGE_TYPE,
        stored_u_ticket_json
    );
}

void FlowApplyUTicket::self_device_recv_u_ticket(UTicket received_u_ticket) {
    try{
        // [STAGE: (R)(VR)]
        // [STAGE: (SR)]
        // no need to optionally _store_received_xxx_u_ticket

        // [STAGE: (VUT)]
        msg_verifier->verify_u_ticket_can_execute(received_u_ticket);
        shared_data->result_message = " -> SUCCESS: VERIFY_UT_CAN_EXECUTE";

        // UT-RT
        if (received_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET
            || received_u_ticket.u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET) \
        {
            //[STAGE: (EO)]
            executor->self_execute_xxx_u_ticket(received_u_ticket);
            // [STAGE: (C)]
            executor->self_change_state(this_device::STATE_DEVICE_WAIT_FOR_UT);
        } else if (received_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_UTICKET
        || received_u_ticket.u_ticket_type == u_ticket::TYPE_SELFACCESS_UTICKET) {
            // [STAGE: (E)]
            RTicket empty_r_ticket;
            executor->self_execute_cr_ke(1, received_u_ticket, empty_r_ticket, "device");

            // [STAGE: (C)]
            executor->self_change_state(this_device::STATE_DEVICE_WAIT_FOR_CRKE2);
        } else {
            throw invalid_argument("should not reach here");
        }

        //error
        // TODO

        //finally
        cout << "Are you sure you want to send the u_ticket to the holder? (y/n): ";

        if (received_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET
            || received_u_ticket.u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET) \
        {
            // [STAGE: (G)(S)]
            self_device_send_r_ticket(
                received_u_ticket.u_ticket_type,
                received_u_ticket.u_ticket_id,
                shared_data->result_message
            );
            
        } else if (received_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_UTICKET
        || received_u_ticket.u_ticket_type == u_ticket::TYPE_SELFACCESS_UTICKET) {
            // [STAGE: (G)(S)]
            flow_open_session->self_device_send_cr_ke_1(shared_data->result_message);
        } else {
            throw invalid_argument("should not reach here");
        }
    } catch(const std::runtime_error& e){
        shared_data->result_message = e.what();
        // [STAGE: (C)]
        executor->self_change_state(this_device::STATE_DEVICE_WAIT_FOR_UT);

        if (received_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET
            || received_u_ticket.u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET) \
        {
            // [STAGE: (G)(S)]
            self_device_send_r_ticket(
                received_u_ticket.u_ticket_type,
                received_u_ticket.u_ticket_id,
                shared_data->result_message
            );
            
        } else if (received_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_UTICKET
        || received_u_ticket.u_ticket_type == u_ticket::TYPE_SELFACCESS_UTICKET) {
            // [STAGE: (G)(S)]
            flow_open_session->self_device_send_cr_ke_1(shared_data->result_message);
        } else {
            throw invalid_argument("should not reach here");
        }
    }    
}

void FlowApplyUTicket::self_device_send_r_ticket(string u_ticket_type, string u_ticket_id, string result_message) {
    // [STAGE: (G)]
    json generated_request;
    generated_request["r_ticket_type"] = u_ticket_type;
    generated_request["device_id"] = shared_data->this_device.device_pub_key;
    generated_request["result"] = result_message;

    if (u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET || u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET) {
        if (result_message.find("SUCCESS") != string::npos) {
            generated_request["audit_start"] = u_ticket_id;
        }
    } else if (u_ticket_type == u_ticket::TYPE_ACCESS_END_UTOKEN) {
        if (result_message.find("SUCCESS") != string::npos) {
            generated_request["audit_start"] = shared_data->current_session.current_u_ticket_id;
            generated_request["audit_end"] = "ACCESS_END";
        }
    } else {
        throw invalid_argument("self_device_send_r_ticket: should not reach here");
    }
    std::string generated_r_ticket_json = msg_generator->self_generate_xxx_r_ticket(generated_request.dump());
    //cout << "debug: generated_r_ticket_json: " << generated_r_ticket_json << "\n";
    // [STAGE: (S)]
    msg_sender->send_xxx_message(
        message::MESSAGE_VERIFY_AND_EXECUTE,
        r_ticket::MESSAGE_TYPE,
        generated_r_ticket_json
    );
    
}

void FlowApplyUTicket::self_holder_recv_r_ticket(RTicket received_r_ticket) {
    //[STAGE: (R)(VR)]
    //[STAGE: (SR)]
    try{
        received_msg_storer->store_received_xxx_r_ticket(received_r_ticket);

        if (received_r_ticket.r_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET || received_r_ticket.r_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET || received_r_ticket.r_ticket_type == u_ticket::TYPE_ACCESS_END_UTOKEN) {
            // [STAGE: (VL)(L)]
            std::string store_u_ticket_json = shared_data->device_table[received_r_ticket.device_id].device_u_ticket_for_owner;
            //[STAGE: (VR)]
            UTicket store_u_ticket = msg_verifier->self_classify_u_ticket_is_defined_type(store_u_ticket_json);

            // [STAGE: (VRT)]
            msg_verifier->verify_u_ticket_has_executed_through_r_ticket(received_r_ticket, store_u_ticket, UTicket());

            // [STAGE: (E)(O)]
            //cout << "debug: r_ticket_type: " << received_r_ticket.r_ticket_type << "\n";
            executor->self_execute_xxx_r_ticket(received_r_ticket, "holder-or-device");
            shared_data->result_message = "-> SUCCESS: VERIFY_UT_HAS_EXECUTED";
            // [STAGE: (C)]
            executor->self_change_state(this_device::STATE_AGENT_WAIT_FOR_UREQ_UREJ_UT_RT);
        } else {
            throw invalid_argument("self_holder_recv_r_ticket: should not reach here");
        }
    }catch(const std::runtime_error& e){
        shared_data->result_message = e.what();
    }
}