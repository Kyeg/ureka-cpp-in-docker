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

TEST(TestFlow3, ApplyAccessUTicketOnDevice) {
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

    target_device_id = iot_device.shared_data->this_device.device_pub_key;
    json temp;
    temp["ALL"] = "allow";
    string generated_task_scope = temp.dump();
    json generated_request3;
    generated_request3["device_id"] = target_device_id;
    generated_request3["holder_id"] = user_agent.shared_data->this_person.person_pub_key;
    generated_request3["u_ticket_type"] = u_ticket::TYPE_SELFACCESS_UTICKET;
    generated_request3["task_scope"] = generated_task_scope;

    user_agent.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_herself(target_device_id, generated_request3.dump());
    cout << "line227\n";

    string generated_command = "HELLO-1";
    user_agent.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id, generated_command);
    cout << "line231\n";

    iot_device.msg_receiver->recv_xxx_message();
    cout << "line234\n";
    user_agent.msg_receiver->recv_xxx_message(); //crke1
    cout << "line236\n";
    iot_device.msg_receiver->recv_xxx_message(); //crke2
    cout << "line238\n";
    user_agent.msg_receiver->recv_xxx_message(); //crke3
    cout << "line240\n";
    // iot_device.msg_receiver->recv_xxx_message(); //cmd u-token
    // cout << "line242\n";
    // user_agent.msg_receiver->recv_xxx_message(); //data r-token
    // cout << "line244\n";
    // iot_device.msg_receiver->recv_xxx_message(); //access end
    // cout << "line246\n";
    // user_agent.msg_receiver->recv_xxx_message(); //return r-ticket
    cout << "line248\n";

    cout << "iot_device.shared_data->result_message: " << user_agent.shared_data->result_message << endl;


    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(user_agent.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->this_device.owner_pub_key, user_agent.shared_data->this_person.person_pub_key);
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_cmd, generated_command);
    //plaintext_data
    ASSERT_EQ(iot_device.shared_data->current_session.plaintext_data, ("DATA: " + generated_command));
    string temp1 = iot_device.shared_data->current_session.current_session_key_str, temp2 = user_agent.shared_data->current_session.current_session_key_str;
    iot_device.shared_data->current_session.current_session_key_str = "";
    user_agent.shared_data->current_session.current_session_key_str = "";
    ASSERT_EQ(temp1, temp2);
    cout << "iot_device.shared_data->current_session.to_json(): " << iot_device.shared_data->current_session.to_json() << endl;
    ASSERT_EQ(iot_device.shared_data->current_session.to_json(), user_agent.shared_data->current_session.to_json());


}


int main () {

    testing::InitGoogleTest();
    // run 4 test separately

    return RUN_ALL_TESTS();

}