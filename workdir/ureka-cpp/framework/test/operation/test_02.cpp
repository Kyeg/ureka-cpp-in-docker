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

TEST(TestFlow2, ApplyOwnershipUTicketOnDevice) {

    // device_manufacturer_server_and_her_device

    DeviceController cloud_server_dm = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_dm");

    cloud_server_dm.executor->self_execute_one_time_initialize_agent_or_server();

    DeviceController iot_device = DeviceController(this_device::IOT_DEVICE, "iot_device");

    // cout << "iot_device.shared_data->this_device.ticket_order: " << iot_device.shared_data->this_device.ticket_order << endl;

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

    //
    // cout<< "result_message :"<< cloud_server_dm.shared_data->result_message << endl;

    DeviceController user_agent = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "user_agent_do");

    user_agent.executor->self_execute_one_time_initialize_agent_or_server();

    string target_device_id = iot_device.shared_data->this_device.device_pub_key;
    int original_device_order = iot_device.shared_data->this_device.ticket_order;
    int original_agent_order = cloud_server_dm.shared_data->device_table[target_device_id].ticket_order;
    // cout << "original_device_order: " << original_device_order << endl;
    // cout << "original_agent_order: " << original_agent_order << endl;
    json generated_request2;
    generated_request2["device_id"] = target_device_id;
    generated_request2["holder_id"] = user_agent.shared_data->this_person.person_pub_key;
    generated_request2["u_ticket_type"] = u_ticket::TYPE_OWNERSHIP_UTICKET;
    // cout << "target_device_id: " << cloud_server_dm.shared_data->device_table[target_device_id].to_json() << endl;

    cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_holder(target_device_id, generated_request2.dump());

    user_agent.msg_receiver->recv_xxx_message();
    cout << "line 118\n";
    user_agent.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id, "");
    cout << "line 120\n";
    iot_device.msg_receiver->recv_xxx_message();
    cout << "line 122\n";
    user_agent.msg_receiver->recv_xxx_message();

    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(user_agent.shared_data->result_message.find("SUCCESS") != string::npos);

    ASSERT_EQ(iot_device.shared_data->this_device.owner_pub_key, user_agent.shared_data->this_person.person_pub_key);
    ASSERT_EQ(iot_device.shared_data->this_device.ticket_order, original_device_order + 1);
    ASSERT_EQ(user_agent.shared_data->device_table[target_device_id].ticket_order, original_agent_order + 1);

    user_agent.flow_issuer_issue_u_ticket->holder_send_r_ticket_to_issuer(target_device_id);
    // cout << "line 152\n";
    cloud_server_dm.msg_receiver->recv_xxx_message();
    // cout << "line 154\n";

    ASSERT_TRUE(cloud_server_dm.shared_data->result_message.find("SUCCESS") != string::npos);
    // ASSERT if cloud_server_dm.shared_data->device_table[target_device_id] exists
    ASSERT_TRUE(cloud_server_dm.shared_data->device_table[target_device_id].device_id == "");
    // cout << "target_device_id: " << cloud_server_dm.shared_data->device_table[target_device_id].to_json() << endl;

}


int main () {

    testing::InitGoogleTest();
    // run 4 test separately

    return RUN_ALL_TESTS();

}