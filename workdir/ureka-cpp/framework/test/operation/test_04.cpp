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

TEST(TestFlow4, ApplyAccessUTicketbyOtherDevice) {
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

    user_agent.flow_issuer_issue_u_ticket->holder_send_r_ticket_to_issuer(target_device_id);
    cloud_server_dm.msg_receiver->recv_xxx_message();

    cout << "fucker heare=================================================================\n";

    DeviceController cloud_server_ep = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_ep");

    cloud_server_ep.executor->self_execute_one_time_initialize_agent_or_server();

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

    cout << "line79\n";
    cloud_server_ep.msg_receiver->recv_xxx_message();
    string generated_command = "HELLO-1";
    cloud_server_ep.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id, generated_command);

    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message(); //crke1
    iot_device.msg_receiver->recv_xxx_message(); //crke2
    cloud_server_ep.msg_receiver->recv_xxx_message(); //crke3

    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(cloud_server_ep.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->this_device.owner_pub_key, user_agent.shared_data->this_person.person_pub_key);
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_cmd, generated_command);
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_data, ("DATA: " + generated_command));
    string temp1 = iot_device.shared_data->current_session.current_session_key_str, temp2 = cloud_server_ep.shared_data->current_session.current_session_key_str;
    iot_device.shared_data->current_session.current_session_key_str = "";
    cloud_server_ep.shared_data->current_session.current_session_key_str = "";
    ASSERT_EQ(temp1, temp2);
    ASSERT_EQ(iot_device.shared_data->current_session.to_json(), cloud_server_ep.shared_data->current_session.to_json());

    iot_device.shared_data->current_session.current_session_key_str = temp1;
    cloud_server_ep.shared_data->current_session.current_session_key_str = temp2;

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    string generated_command2 = "HELLO-2";
    cloud_server_ep.flow_issue_u_token->holder_send_cmd(target_device_id, generated_command2);

    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message(); //cmd u-token

    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(cloud_server_ep.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_cmd, generated_command2);
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_data, ("DATA: " + generated_command2));
    temp1 = iot_device.shared_data->current_session.current_session_key_str, temp2 = cloud_server_ep.shared_data->current_session.current_session_key_str;
    iot_device.shared_data->current_session.current_session_key_str = "";
    cloud_server_ep.shared_data->current_session.current_session_key_str = "";
    ASSERT_EQ(temp1, temp2);
    ASSERT_EQ(iot_device.shared_data->current_session.to_json(), cloud_server_ep.shared_data->current_session.to_json());

    //key back
    iot_device.shared_data->current_session.current_session_key_str = temp1;
    cloud_server_ep.shared_data->current_session.current_session_key_str = temp2;

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    generated_command2 = "HELLO-3";
    cloud_server_ep.flow_issue_u_token->holder_send_cmd(target_device_id, generated_command2);

    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message(); //cmd u-token

    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(cloud_server_ep.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_cmd, generated_command2);
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_data, ("DATA: " + generated_command2));
    temp1 = iot_device.shared_data->current_session.current_session_key_str, temp2 = cloud_server_ep.shared_data->current_session.current_session_key_str;
    iot_device.shared_data->current_session.current_session_key_str = "";
    cloud_server_ep.shared_data->current_session.current_session_key_str = "";
    ASSERT_EQ(temp1, temp2);
    ASSERT_EQ(iot_device.shared_data->current_session.to_json(), cloud_server_ep.shared_data->current_session.to_json());

    //key back
    iot_device.shared_data->current_session.current_session_key_str = temp1;
    cloud_server_ep.shared_data->current_session.current_session_key_str = temp2;

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    original_device_order = iot_device.shared_data->this_device.ticket_order;
    original_agent_order = cloud_server_ep.shared_data->device_table[target_device_id].ticket_order;
    generated_command2 = "ACCESS_END";

    cloud_server_ep.flow_issue_u_token->holder_send_cmd(target_device_id, generated_command2, true);

    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_ep.msg_receiver->recv_xxx_message(); //access end

    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(cloud_server_ep.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->this_device.ticket_order, original_device_order + 1);
    ASSERT_EQ(cloud_server_ep.shared_data->device_table[target_device_id].ticket_order, original_agent_order + 1);

    cloud_server_ep.flow_issuer_issue_u_ticket->holder_send_r_ticket_to_issuer(target_device_id);

    user_agent.msg_receiver->recv_xxx_message();

    ASSERT_TRUE(user_agent.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(user_agent.shared_data->device_table[target_device_id].ticket_order == original_agent_order + 1);

}

int main () {

    testing::InitGoogleTest();
    // run 4 test separately
    /* {
    "message_operation": "MESSAGE_VERIFY_AND_EXECUTE",
    "message_str": "{\n    \"device_id\": \"no_id\",\n    \"holder_id\": \"-----BEGIN PUBLIC KEY-----\\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEJ0D2uOh1e/u/rf/BIn5gcBX+S5fCrzyE\\ndLWJ/goIdhSDC0pmUw57YBWymHhK6jEyTJ+ql3+yupGpnj1bZFFYdA==\\n-----END PUBLIC KEY-----\\n\",\n    \"protocol_version\": \"UREKA-1.0\",\n    \"ticket_order\": 0,\n    \"u_ticket_id\": \"ddf57913ec7cb1938bfb061b0e02c5d793d37d74e68ddd6e74bf2821e496c1a2\",\n    \"u_ticket_type\": \"INITIALIZATION\"\n}",
    "message_type": "UTICKET"
} */
    json temp;
    temp["device_id"] = "no_id";
    temp["holder_id"] = "-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEJ0D2uOh1e/u/rf/BIn5gcBX+S5fCrzyE\ndLWJ/goIdhSDC0pmUw57YBWymHhK6jEyTJ+ql3+yupGpnj1bZFFYdA==\n-----END PUBLIC KEY-----\n";
    temp["protocol_version"] = "UREKA-1.0";
    temp["ticket_order"] = 0;
    temp["u_ticket_id"] = "ddf57913ec7cb1938bfb061b0e02c5d793d37d74e68ddd6e74bf2821e496c1a2";
    temp["u_ticket_type"] = "INITIALIZATION";
    json ticket;
    ticket["message_operation"] = "MESSAGE_VERIFY_AND_EXECUTE";
    ticket["message_str"] = temp.dump();
    ticket["message_type"] = "UTICKET";
    return RUN_ALL_TESTS();

}
