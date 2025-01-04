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

TEST(TestFlow5, ManufacturerInitFail) {
    DeviceController cloud_server_dm = DeviceController(this_device::USER_AGENT_OR_CLOUD_SERVER, "cloud_server_dm");

    cloud_server_dm.executor->self_execute_one_time_initialize_agent_or_server();

    try{
        cloud_server_dm.executor->self_execute_one_time_initialize_agent_or_server();
        throw std::runtime_error("should not be able to initialize");
    }catch(const std::exception& e){
        cout << "Exception: " << e.what() << endl;
        string error_message = "-> FAILURE: VERIFY_TICKET_ORDER: USER-AGENT-OR-CLOUD-SERVER ALREADY INITIALIZED";
        ASSERT_EQ(e.what(), error_message);
    }
}

int main () {

    testing::InitGoogleTest();
    // run 4 test separately

    return RUN_ALL_TESTS();

}