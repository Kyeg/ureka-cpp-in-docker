#ifndef MSG_VERIFIER_MESSAGE_HPP
#define MSG_VERIFIER_MESSAGE_HPP
#include <string>
#include "../../model/data_model/this_device.hpp"
#include "../../model/message_model/message.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class MessageVerifier {
    public:
    ThisDevice this_device;

    MessageVerifier(ThisDevice this_device){
        this->this_device = this_device;
    }

    Message verify_json_schema(string arbitrary_json);

    Message verify_message_operation(Message message_in);

    Message verify_message_type(Message message_in);

    Message verify_message_str(Message message_in);
};

#endif // MSG_VERIFIER_MESSAGE_HPP