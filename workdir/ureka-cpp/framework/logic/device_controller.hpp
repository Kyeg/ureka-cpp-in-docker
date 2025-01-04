#ifndef DEVICE_CONTROLLER_HPP
#define DEVICE_CONTROLLER_HPP

#include "../model/shared_data.hpp"
#include "../resource/storage/simple_storage.hpp"
// #include "measure_helper.hpp"
#include "stage_worker/received_msg_storer.hpp"
#include "stage_worker/msg_verifier.hpp"
#include "stage_worker/executor.hpp"
#include "stage_worker/msg_generator.hpp"
#include "stage_worker/generated_msg_storer.hpp"
#include "stage_worker/msg_sender.hpp"
#include "stage_worker/msg_receiver.hpp"
#include "pipeline_flow/flow_issue_u_ticket.hpp"
#include "pipeline_flow/flow_apply_u_ticket.hpp"
#include "pipeline_flow/flow_open_session.hpp"
#include "pipeline_flow/flow_issue_u_token.hpp"
// #include "simple_logger.hpp"
#include <string>

// 前向声明
class SharedData;
class SimpleStorage;
class MeasureHelper;
class ReceivedMsgStorer;
class MsgVerifier;
class Executor;
class MsgGenerator;
class GeneratedMsgStorer;
class MsgSender;
class MsgReceiver;
class FlowIssueUTicket;
class FlowApplyUTicket;
class FlowOpenSession;
class FlowIssueUToken;

class DeviceController {
public:
    DeviceController(const std::string& device_type = "", const std::string& device_name = "");
    ~DeviceController();
    void reboot_device();


    SharedData* shared_data;
    SimpleStorage* simple_storage;
    MeasureHelper* measure_helper;

    // Stage Workers
    ReceivedMsgStorer* received_msg_storer;
    MsgVerifier* msg_verifier;
    Executor* executor;
    MsgGenerator* msg_generator;
    GeneratedMsgStorer* generated_msg_storer;
    MsgSender* msg_sender;
    MsgReceiver* msg_receiver;

    // Flows
    FlowIssueUTicket* flow_issuer_issue_u_ticket;
    FlowOpenSession* flow_open_session;
    FlowApplyUTicket* flow_apply_u_ticket;
    FlowIssueUToken* flow_issue_u_token;

    void initialize();
    void cleanup();
};

#endif