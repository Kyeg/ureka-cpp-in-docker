#ifndef FLOW_ISSUE_U_TICKET_HPP
#define FLOW_ISSUE_U_TICKET_HPP
#include "../../model/shared_data.hpp"
// #include "../stage_worker/measure_helper.hpp"
#include "../stage_worker/received_msg_storer.hpp"
#include "../stage_worker/msg_verifier.hpp"
#include "../stage_worker/executor.hpp"
#include "../stage_worker/msg_generator.hpp"
#include "../stage_worker/generated_msg_storer.hpp"
#include "../stage_worker/msg_sender.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../model/message_model/r_ticket.hpp"
#include "../../resource/logger/simple_logger.hpp"
#include "../../environment.hpp"
#include <string>

class FlowIssueUTicket {
public:
    FlowIssueUTicket(
        SharedData* shared_data,
        ReceivedMsgStorer* received_msg_storer,
        MsgVerifier* msg_verifier,
        Executor* executor,
        MsgGenerator* msg_generator,
        GeneratedMsgStorer* generated_msg_storer,
        MsgSender* msg_sender
    );

    void issuer_issue_u_ticket_to_herself(const std::string& device_id, const std::string& arbitrary_dict);
    void issuer_issue_u_ticket_to_holder(const std::string& device_id, const std::string& arbitrary_dict);
    void self_holder_recv_u_ticket(const UTicket& received_u_ticket);
    void holder_send_r_ticket_to_issuer(const std::string& device_id);
    void self_issuer_recv_r_ticket(const RTicket& received_r_ticket);

private:
    SharedData* shared_data;
    ReceivedMsgStorer* received_msg_storer;
    MsgVerifier* msg_verifier;
    Executor* executor;
    MsgGenerator* msg_generator;
    GeneratedMsgStorer* generated_msg_storer;
    MsgSender* msg_sender;
};

#endif