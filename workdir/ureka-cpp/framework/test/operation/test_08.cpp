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

TEST(TestFlow8, AttakerForgeToApplyUticket) {
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
    

    DeviceController cloud_server_atk = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_atk");
    cloud_server_atk.executor->self_execute_one_time_initialize_agent_or_server();

    string target_device_id = iot_device.shared_data->this_device.device_pub_key;

    string intercepted_uticket_json = cloud_server_dm.shared_data->device_table[target_device_id].device_u_ticket_for_owner;
    string intercepted_rticket_json = cloud_server_atk.shared_data->device_table[target_device_id].device_r_ticket_for_owner;

    cloud_server_atk.shared_data->device_table[target_device_id] = OtherDevice(target_device_id, intercepted_uticket_json, intercepted_rticket_json, 1);

    generated_request.clear();
    generated_request["device_id"] = target_device_id;
    generated_request["holder_id"] = cloud_server_atk.shared_data->this_person.person_pub_key;    
    generated_request["u_ticket_type"] = u_ticket::TYPE_OWNERSHIP_UTICKET;

    string generated_u_ticket_json = cloud_server_atk.msg_generator->self_generate_xxx_u_ticket(generated_request.dump());

    //forge to apply u ticket
    cloud_server_atk.flow_issuer_issue_u_ticket->self_holder_recv_u_ticket(uticket_from_json_str(generated_u_ticket_json));
    
    cloud_server_atk.flow_apply_u_ticket->holder_apply_u_ticket(target_device_id);
    iot_device.msg_receiver->recv_xxx_message();
    cloud_server_atk.msg_receiver->recv_xxx_message();
    string result_message1 = "-> FAILURE: SELF_VERIFY_ISSUER_SIGNATURE_ON_U_TICKET";
    string result_message2 = "-> FAILURE: VERIFY_RESULT";
    cout << iot_device.shared_data->result_message << endl;
    cout << cloud_server_atk.shared_data->result_message << endl;
    bool res = iot_device.shared_data->result_message.find(result_message1) != string::npos;
    bool res2 = cloud_server_atk.shared_data->result_message.find(result_message2) != string::npos;
    ASSERT_EQ(res, true);
    ASSERT_EQ(res2, true);
    
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}