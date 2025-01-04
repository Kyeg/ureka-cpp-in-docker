#ifndef FLOW_ISSUE_U_TOKEN_HPP
#define FLOW_ISSUE_U_TOKEN_HPP

#include "../../model/shared_data.hpp"
#include "../stage_worker/received_msg_storer.hpp"
#include "../stage_worker/msg_verifier.hpp"
#include "../stage_worker/executor.hpp"
#include "../stage_worker/msg_generator.hpp"
#include "../stage_worker/generated_msg_storer.hpp"
#include "../stage_worker/msg_sender.hpp"
#include "flow_apply_u_ticket.hpp"


class FlowIssueUToken {
private:
    SharedData* shared_data;
    //MeasureHelper measure_helper;
    ReceivedMsgStorer* received_msg_storer;
    MsgVerifier* msg_verifier;
    Executor* executor;
    MsgGenerator* msg_generator;
    GeneratedMsgStorer* generated_msg_storer;
    MsgSender* msg_sender;
    FlowApplyUTicket* flow_apply_u_ticket;
public:

    FlowIssueUToken(SharedData* shared_data, ReceivedMsgStorer* received_msg_storer, MsgVerifier* msg_verifier, Executor* executor, MsgGenerator* msg_generator, GeneratedMsgStorer* generated_msg_storer, MsgSender* msg_sender, FlowApplyUTicket* flow_apply_u_ticket){
        this->shared_data = shared_data;
        // this->measure_helper = measure_helper;
        this->received_msg_storer = received_msg_storer;
        this->msg_verifier = msg_verifier;
        this->executor = executor;
        this->msg_generator = msg_generator;
        this->generated_msg_storer = generated_msg_storer;
        this->msg_sender = msg_sender;
        this->flow_apply_u_ticket = flow_apply_u_ticket;
    }
    
    void holder_send_cmd(const std::string& device_id, const std::string& cmd, bool access_end = false);
    void self_device_recv_cmd(const UTicket& received_u_token);
    void self_device_send_data(const std::string& result_message);
    void self_holder_recv_data(const RTicket& received_r_token);
};

// flow_issue_u_token = new FlowIssueUToken(shared_data, received_msg_storer, msg_verifier, executor, msg_generator, generated_msg_storer, msg_sender, flow_apply_u_ticket);


#endif