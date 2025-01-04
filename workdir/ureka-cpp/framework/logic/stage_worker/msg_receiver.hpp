#ifndef MSG_RECEIVER_HPP
#define MSG_RECEIVER_HPP

#include <queue>
#include "../../model/shared_data.hpp"
#include "msg_verifier.hpp"
#include "executor.hpp"
#include "msg_sender.hpp"
#include "../pipeline_flow/flow_issue_u_ticket.hpp"
#include "../pipeline_flow/flow_apply_u_ticket.hpp"
#include "../pipeline_flow/flow_open_session.hpp"
#include "../pipeline_flow/flow_issue_u_token.hpp"
#include "../../environment.hpp"
// #include "../../resource/stora/simple_logger.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../model/message_model/r_ticket.hpp"

class DeviceController;

class MsgReceiver {
public:
    MsgReceiver(
        SharedData* shared_data,
        // MeasureHelper* measure_helper,
        MsgVerifier* msg_verifier,
        Executor* executor,
        MsgSender* msg_sender,
        FlowIssueUTicket* flow_issuer_issue_u_ticket,
        FlowApplyUTicket* flow_apply_u_ticket,
        FlowOpenSession* flow_open_session,
        FlowIssueUToken* flow_issue_u_token
    );

    void create_simulated_comm_connection();
    void accept_bluetooth_comm();
    void close_bluetooth_connection();
    void close_bluetooth_acception();
    void recv_xxx_message();

private:

    SharedData* shared_data;
    // MeasureHelper* measure_helper_;
    MsgVerifier* msg_verifier;
    Executor* executor;
    MsgSender* msg_sender;
    FlowIssueUTicket* flow_issuer_issue_u_ticket;
    FlowIssueUToken* flow_issue_u_token;
    FlowApplyUTicket* flow_apply_u_ticket;
    FlowOpenSession* flow_open_session;
};

#endif