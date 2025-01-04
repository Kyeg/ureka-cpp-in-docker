#ifndef MSG_SENDER_HPP
#define MSG_SENDER_HPP


#include "../../model/shared_data.hpp"
#include "../../environment.hpp"
#include "../../resource/logger/simple_logger.hpp"
#include "../../model/message_model/message.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../model/message_model/r_ticket.hpp"
// #include "measure_helper.hpp"
#include <string>

class MsgSender {
public:
    MsgSender(SharedData* shared_data);

    // 模拟通信方法

    void start_simulated_comm();
    void complete_simulated_comm();
    void wait_simulated_comm_completed();

    // 蓝牙通信方法

    void start_bluetooth_comm();
    void complete_bluetooth_comm();
    void connect_bluetooth_comm();
    void close_bluetooth_connection();


    void send_xxx_message(const std::string& message_operation, 
                        const std::string& message_type, 
                        const std::string& sent_message_json);
private:
    SharedData* shared_data;
    // MeasureHelper* measure_helper;
};

#endif