#include "msg_verifier.hpp"
#include <string>
#include <iostream>

using namespace std;
using json = nlohmann::json;

pair<UTicket, RTicket> MsgVerifier::self_classify_message_is_defined_type(string arbitrary_json) {
    cout << "info " << shared_data->this_device.device_name << " is classifying message..." << "\n";

    MessageVerifier message_verifier(shared_data->this_device);

    Message message_in = message_verifier.verify_json_schema(arbitrary_json);
    message_in = message_verifier.verify_message_operation(message_in);
    message_in = message_verifier.verify_message_type(message_in);
    message_in = message_verifier.verify_message_str(message_in);


    if (message_in.message_type == u_ticket::MESSAGE_TYPE) {
        return make_pair(self_classify_u_ticket_is_defined_type(message_in.message_str), RTicket());
    } else if (message_in.message_type == r_ticket::MESSAGE_TYPE) {
        return make_pair(UTicket(), self_classify_r_ticket_is_defined_type(message_in.message_str));
    } else {
        throw invalid_argument("message type is not defined");
    }

}

UTicket MsgVerifier::self_classify_u_ticket_is_defined_type(string arbitrary_json) {
    cout << "info" << shared_data->this_device.device_name << "is classifying u_ticket..." << "\n";

    ThisDevice this_device;
    UTicketVerifier u_ticket_verifier(this_device);

    // cout << "info: " << arbitrary_json << "\n";

    UTicket u_ticket_in = u_ticket_verifier.verify_json_schema(arbitrary_json);
    u_ticket_in = u_ticket_verifier.verify_protocol_version(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_u_ticket_id(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_u_ticket_type(u_ticket_in);
    u_ticket_in = u_ticket_verifier.has_device_id(u_ticket_in);

    string u_ticket_str = u_ticket_in.to_json_str();

    return u_ticket_in;

}

RTicket MsgVerifier::self_classify_r_ticket_is_defined_type(string arbitrary_json) {
    cout << "info" << shared_data->this_device.device_name << "is classifying r_ticket..." << "\n";


    RTicketVerifier r_ticket_verifier(shared_data->this_device);

    RTicket r_ticket_in = r_ticket_verifier.verify_json_schema(arbitrary_json);
    r_ticket_in = r_ticket_verifier.verify_protocol_version(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_r_ticket_id(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_r_ticket_type(r_ticket_in);
    r_ticket_in = r_ticket_verifier.has_device_id(r_ticket_in);

    return r_ticket_in;

}

void MsgVerifier::verify_u_ticket_can_execute(UTicket u_ticket_in) {
    // cout << "show me your ass: " << u_ticket_in.to_json_str();
    cout << "info" << shared_data->this_device.device_name << "is verifying u_ticket can execute..." << "\n";


    UTicketVerifier u_ticket_verifier(shared_data->this_device);

    u_ticket_in = u_ticket_verifier.verify_device_id(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_ticket_order(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_holder_id(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_task_scope(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_ps(u_ticket_in);
    u_ticket_in = u_ticket_verifier.verify_issuer_signature(u_ticket_in);

    return;

}

void MsgVerifier::verify_u_ticket_has_executed_through_r_ticket(RTicket r_ticket_in, UTicket audit_start_ticket, UTicket audit_end_ticket) {
    cout << "info" << shared_data->this_device.device_name << "is verifying u_ticket has executed through r_ticket..." << "\n";

    RTicketVerifier r_ticket_verifier(shared_data->this_device, audit_start_ticket, audit_end_ticket, shared_data->device_table, shared_data->current_session);

    r_ticket_in = r_ticket_verifier.verify_device_id(r_ticket_in);

    //cout << "debug: " << "r_ticket_in result: " << r_ticket_in.result << "\n";
    //cout << "debug: " << "r_ticket_in: " << r_ticket_in.to_json_str() << "\n";
    r_ticket_in = r_ticket_verifier.verify_result(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_ticket_order(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_audit_start(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_audit_end(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_cr_key(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_ps(r_ticket_in);
    r_ticket_in = r_ticket_verifier.verify_device_signature(r_ticket_in);

    return;

}

void MsgVerifier::verify_cmd_is_in_task_scope(string cmd) {
    string success_msg = "-> SUCCESS: VERIFY_CMD_IN_TASK_SCOPE";
    string failure_msg = "-> FAILURE: VERIFY_CMD_IN_TASK_SCOPE";

    json task_scope = json::parse(shared_data->current_session.current_task_scope);

    cout << "info: I'm watching you: " << task_scope << "\n";

    // if (task_scope.find("ALL") == "allow")

    if (task_scope.find("ALL") != task_scope.end() && task_scope["ALL"] == "allow") {
        cout << success_msg << "\n";
    } else if (cmd == "HELLO-1" && (task_scope.find("SAY-HELLO-1") != task_scope.end() && task_scope["SAY-HELLO-1"] == "allow")) {
        cout << success_msg << "\n";
    } else if (cmd == "HELLO-2" && (task_scope.find("SAY-HELLO-2") != task_scope.end() && task_scope["SAY-HELLO-2"] == "allow")) {
        cout << success_msg << "\n";
    } else if (cmd == "HELLO-3" && (task_scope.find("SAY-HELLO-3") != task_scope.end() && task_scope["SAY-HELLO-3"] == "allow")) {
        cout << success_msg << "\n";
    } else {
        cout << failure_msg << "\n";
        throw std::runtime_error(failure_msg);
    }

    return;

}