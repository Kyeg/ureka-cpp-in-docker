#ifndef SHARED_DATA_HPP
#define SHARED_DATA_HPP

#include <string>
#include <map>
#include <thread>
#include "data_model/this_device.hpp"
#include "data_model/current_session.hpp"
#include "data_model/this_person.hpp"
#include "data_model/other_device.hpp"
#include "../resource/communication/simulated_comm_channel.hpp"
// #include "simulated_comm_channel.hpp"

// #ifdef HAS_PYBLUEZ
// #include "bluetooth_service.hpp"
// #endif

using namespace std;

class SharedData {
public:

    SharedData() : 
        this_device(ThisDevice()),
        current_session(CurrentSession()),
        this_person(ThisPerson()),
        device_table(map<string, OtherDevice>()),
        state(""),
        received_message_json(""),
        result_message(""),
        measure_rec(map<string, string>()),
        simulated_comm_completed_flag(false)
        // accept_socket(AcceptSocket()),
        // connecting_worker(ConnectingWorker()),
        // connection_socket(ConnectionSocket()),
        // bluetooth_comm_completed_flag(false)
    {};

    SharedData(const SharedData& shared_data) : 
        this_device(shared_data.this_device),
        current_session(shared_data.current_session),
        this_person(shared_data.this_person),
        device_table(shared_data.device_table),
        state(shared_data.state),
        received_message_json(shared_data.received_message_json),
        result_message(shared_data.result_message),
        measure_rec(shared_data.measure_rec),
        simulated_comm_completed_flag(shared_data.simulated_comm_completed_flag)
        // accept_socket(shared_data.accept_socket),
        // connecting_worker(shared_data.connecting_worker),
        // connection_socket(shared_data.connection_socket),
        // bluetooth_comm_completed_flag(shared_data.bluetooth_comm_completed_flag)
    {};
    // 数据模型 (持久化)
    ThisDevice this_device;
    CurrentSession current_session;
    // 数据模型 (持久化: 仅用于用户代理或云服务器)
    ThisPerson this_person;
    map<string, OtherDevice> device_table;
    // 数据模型 (仅内存)
    string state;

    // CLI 输出
    string received_message_json;
    string result_message;

    // 测量记录
    map<string, string> measure_rec;

    // 资源 (模拟通信)
    // SimulatedCommChannel simulated_comm_channel;
    // SimulatedCommChannel simulated_comm_channel;
    // std::thread simulated_comm_receiver_thread;
    bool simulated_comm_completed_flag;

    // 资源 (蓝牙通信)
// #ifdef HAS_PYBLUEZ
//     AcceptSocket accept_socket;
//     ConnectingWorker connecting_worker;
//     ConnectionSocket connection_socket;
//     bool bluetooth_comm_completed_flag;
// #endif
};

#endif // SHARED_DATA_HPP