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

TEST(TestFlow1, TestFlowIssueUTicket) {
    /* DeviceController cloud_server_dm = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_dm");

    cloud_server_dm.executor->self_execute_one_time_initialize_agent_or_server();

    DeviceController iot_device = DeviceController(this_device::IOT_DEVICE, "iot_device");

    // cout << "iot_device.shared_data->this_device.ticket_order: " << iot_device.shared_data->this_device.ticket_order << endl;

    ASSERT_EQ(iot_device.shared_data->this_device.ticket_order, 0);

    string id_for_no_init_device = "no_id";

    json generated_request;
    generated_request["device_id"] = id_for_no_init_device;
    generated_request["holder_id"] = cloud_server_dm.shared_data->this_person.person_pub_key;
    generated_request["u_ticket_type"] = u_ticket::TYPE_INITIALIZATION_UTICKET;

    // cout << "before issuer issuer issue u ticket to herself" << endl;

    cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_herself(id_for_no_init_device, generated_request.dump());

    // cout << "cloud_server_dm.shared_data.device_table[\"no_id\"]" << cloud_server_dm.shared_data->device_table["no_id"].device_u_ticket_for_owner << endl;
 
    cloud_server_dm.flow_apply_u_ticket->holder_apply_u_ticket(id_for_no_init_device, ""); */
    DeviceController iot_device = DeviceController(this_device::IOT_DEVICE, "iot_device");

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
	SIMULATED_GLOBAL_CHANNEL = ticket.dump();

    json hello = json::parse(SIMULATED_GLOBAL_CHANNEL);

    iot_device.msg_receiver->recv_xxx_message();
    /* cloud_server_dm.msg_receiver->recv_xxx_message();

    ASSERT_TRUE(cloud_server_dm.shared_data->result_message.find("SUCCESS") != string::npos);
    ASSERT_TRUE(iot_device.shared_data->result_message.find("SUCCESS") != string::npos);

    // assert self.iot_device.shared_data.this_device.ticket_order == 1
    ASSERT_EQ(iot_device.shared_data->this_device.ticket_order, 1);
    ASSERT_NE(iot_device.shared_data->this_device.device_priv_key, "");
    ASSERT_NE(iot_device.shared_data->this_device.device_pub_key, "");
    ASSERT_EQ(iot_device.shared_data->this_device.owner_pub_key, cloud_server_dm.shared_data->this_person.person_pub_key);
    ASSERT_EQ(iot_device.shared_data->this_person.person_priv_key, "");
    ASSERT_EQ(iot_device.shared_data->this_person.person_pub_key, ""); */
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}