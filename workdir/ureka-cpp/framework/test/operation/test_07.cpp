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

TEST(TestFlow6, TestReInitIotDevice) {
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

    // cout << "before issuer issuer issue u ticket to herself" << endl;

    cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_herself(id_for_no_init_device, generated_request.dump());

    // cout << "cloud_server_dm.shared_data.device_table[\"no_id\"]" << cloud_server_dm.shared_data->device_table["no_id"].device_u_ticket_for_owner << endl;
 
    cloud_server_dm.flow_apply_u_ticket->holder_apply_u_ticket(id_for_no_init_device, "");
    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_dm.msg_receiver->recv_xxx_message();
    string result_message = "-> FAILURE: VERIFY_TICKET_ORDER: IOT_DEVICE ALREADY INITIALIZED";

    // try {
        cloud_server_dm.flow_issuer_issue_u_ticket->issuer_issue_u_ticket_to_herself(id_for_no_init_device, generated_request.dump());

        cloud_server_dm.flow_apply_u_ticket->holder_apply_u_ticket(id_for_no_init_device, "");
        iot_device.msg_receiver->recv_xxx_message();
        ASSERT_EQ(iot_device.shared_data->result_message, result_message);
    // }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}