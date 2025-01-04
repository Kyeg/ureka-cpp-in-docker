#include <gtest/gtest.h>
#include <iostream>
#include <string>
// #include "flow_open_seesion.hpp"
#include "../../logic/pipeline_flow/flow_issue_u_ticket.hpp"
#include "../../logic/pipeline_flow/flow_apply_u_ticket.hpp"
#include "../../logic/device_controller.hpp"
#include "../../model/data_model/this_person.hpp"
#include "../../model/data_model/this_device.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

TEST(TestFlow16, TestFailWhenInterceptAndReuseUToken) {
    DeviceController cloud_server_dm = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_dm");

    cloud_server_dm.executor->self_execute_one_time_initialize_agent_or_server();

    DeviceController iot_device = DeviceController(this_device::IOT_DEVICE, "iot_device");

    ASSERT_EQ(iot_device.shared_data->this_device.ticket_order, 0);

    string id_for_no_init_device = "no_id";

    json generated_request;
    generated_request["device_id"] = id_for_no_init_device;
    generated_request["holder_id"] = cloud_server_dm.shared_data->this_person.person_pub_key;
    generated_request["u_ticket_type"] = u_ticket::TYPE_INITIALIZATION_UTICKET;

    cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_herself(id_for_no_init_device, generated_request.dump());

    cloud_server_dm.flow_apply_u_ticket->holder_apply_u_ticket(id_for_no_init_device, "");

    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_dm.msg_receiver->recv_xxx_message();



    DeviceController user_agent = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "user_agent_do");

    user_agent.executor->self_execute_one_time_initialize_agent_or_server();

    string target_device_id = iot_device.shared_data->this_device.device_pub_key;
    int original_device_order = iot_device.shared_data->this_device.ticket_order;
    int original_agent_order = cloud_server_dm.shared_data->device_table[target_device_id].ticket_order;
    json generated_request2;
    generated_request2["device_id"] = target_device_id;
    generated_request2["holder_id"] = user_agent.shared_data->this_person.person_pub_key;
    generated_request2["u_ticket_type"] = u_ticket::TYPE_OWNERSHIP_UTICKET;

    cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_holder(target_device_id, generated_request2.dump());

    user_agent.msg_receiver->recv_xxx_message();
    user_agent.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id, "");
    iot_device.msg_receiver->recv_xxx_message();
    user_agent.msg_receiver->recv_xxx_message();

    DeviceController cloud_server_ep = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_ep");
    cloud_server_ep.executor->self_execute_one_time_initialize_agent_or_server();

    DeviceController cloud_server_atk = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_atk");
    cloud_server_atk.executor->self_execute_one_time_initialize_agent_or_server();

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    json temp;
    temp["ALL"] = "allow";
    string generated_task_scope = temp.dump();
    json generated_request3;
    generated_request3["device_id"] = target_device_id;
    generated_request3["holder_id"] = cloud_server_ep.shared_data->this_person.person_pub_key;
    generated_request3["u_ticket_type"] = u_ticket::TYPE_ACCESS_UTICKET;
    generated_request3["task_scope"] = generated_task_scope;

    user_agent.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_holder(target_device_id, generated_request3.dump());
    cloud_server_ep.msg_receiver->recv_xxx_message();

    string generated_command = "HELLO-2";
    cloud_server_ep.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id, generated_command);
    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message();
    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message();

    cout << "iot state is: " << iot_device.shared_data->state << endl;

    cloud_server_ep.flow_issue_u_token->holder_send_cmd(target_device_id, generated_command);
    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message();
    
    cout << "line86\n";

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    string intercepted_uticket = cloud_server_ep.shared_data->device_table[target_device_id].device_u_ticket_for_owner;

    cloud_server_atk.flow_issuer_issue_u_ticket->self_holder_recv_u_ticket(uticket_from_json_str(intercepted_uticket));
    cout << "what is the received message: " << iot_device.shared_data->received_message_json << endl;
    string intercepted_utoken = iot_device.shared_data->received_message_json;
    cout << "line 98\n";
    cloud_server_atk.executor->self_change_state("STATE_AGENT_WAIT_FOR_DATA");
    cout << "line 100\n";
    cloud_server_atk.msg_sender->send_xxx_message(
        message::MESSAGE_VERIFY_AND_EXECUTE,
        u_ticket::MESSAGE_TYPE,
        intercepted_utoken
    );

    cout << "line 106\n";

    iot_device.msg_receiver->recv_xxx_message();

    cout << "what's in the received message: " << iot_device.shared_data->result_message << endl;

    ASSERT_TRUE(iot_device.shared_data->result_message.find("FAILURE") != string::npos);

    try {
        cloud_server_atk.msg_receiver->recv_xxx_message();
    } catch (const std::exception& e) {
        cout << "Exception: " << e.what() << endl;
        string expected = "-> FAILURE: VERIFY_RESULT";
        ASSERT_EQ(e.what(), expected);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}