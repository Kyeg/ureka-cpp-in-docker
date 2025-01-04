#include "nlohmann/json.hpp"
#include <string>
#include "current_session.hpp"

using namespace std;
using json = nlohmann::json;

string CurrentSession::to_json() {
    json j;
    if (!current_u_ticket_id.empty()) j["current_u_ticket_id"] = current_u_ticket_id;
    if (!current_device_id.empty()) j["current_device_id"] = current_device_id;
    if (!current_holder_id.empty()) j["current_holder_id"] = current_holder_id;
    if (!current_task_scope.empty()) j["current_task_scope"] = current_task_scope;

    if (!challenge_1.empty()) j["challenge_1"] = challenge_1;
    if (!challenge_2.empty()) j["challenge_2"] = challenge_2;
    if (!key_exchange_salt_1.empty()) j["key_exchange_salt_1"] = key_exchange_salt_1;
    if (!key_exchange_salt_2.empty()) j["key_exchange_salt_2"] = key_exchange_salt_2;

    if (!current_session_key_str.empty()) j["current_session_key_str"] = current_session_key_str;
    if (!plaintext_cmd.empty()) j["plaintext_cmd"] = plaintext_cmd;
    if (!associated_plaintext_cmd.empty()) j["associated_plaintext_cmd"] = associated_plaintext_cmd;
    if (!iv_cmd.empty()) j["iv_cmd"] = iv_cmd;
    if (!ciphertext_cmd.empty()) j["ciphertext_cmd"] = ciphertext_cmd;
    if (!gcm_authentication_tag_cmd.empty()) j["gcm_authentication_tag_cmd"] = gcm_authentication_tag_cmd;

    if (!plaintext_data.empty()) j["plaintext_data"] = plaintext_data;
    if (!associated_plaintext_data.empty()) j["associated_plaintext_data"] = associated_plaintext_data;
    if (!iv_data.empty()) j["iv_data"] = iv_data;
    if (!ciphertext_data.empty()) j["ciphertext_data"] = ciphertext_data;
    return j.dump();
}

CurrentSession json_to_current_session(json j) {
    CurrentSession session;
    
    if (j.find("current_u_ticket_id") != j.end()) session.current_u_ticket_id = j["current_u_ticket_id"];
    if (j.find("current_device_id") != j.end()) session.current_device_id = j["current_device_id"];
    if (j.find("current_holder_id") != j.end()) session.current_holder_id = j["current_holder_id"];
    if (j.find("current_task_scope") != j.end()) session.current_task_scope = j["current_task_scope"];

    if (j.find("challenge_1") != j.end()) session.challenge_1 = j["challenge_1"];
    if (j.find("challenge_2") != j.end()) session.challenge_2 = j["challenge_2"];
    if (j.find("key_exchange_salt_1") != j.end()) session.key_exchange_salt_1 = j["key_exchange_salt_1"];
    if (j.find("key_exchange_salt_2") != j.end()) session.key_exchange_salt_2 = j["key_exchange_salt_2"];

    if (j.find("current_session_key_str") != j.end()) session.current_session_key_str = j["current_session_key_str"];
    if (j.find("plaintext_cmd") != j.end()) session.plaintext_cmd = j["plaintext_cmd"];
    if (j.find("associated_plaintext_cmd") != j.end()) session.associated_plaintext_cmd = j["associated_plaintext_cmd"];
    if (j.find("iv_cmd") != j.end()) session.iv_cmd = j["iv_cmd"];
    if (j.find("ciphertext_cmd") != j.end()) session.ciphertext_cmd = j["ciphertext_cmd"];
    if (j.find("gcm_authentication_tag_cmd") != j.end()) session.gcm_authentication_tag_cmd = j["gcm_authentication_tag_cmd"];

    if (j.find("plaintext_data") != j.end()) session.plaintext_data = j["plaintext_data"];
    if (j.find("associated_plaintext_data") != j.end()) session.associated_plaintext_data = j["associated_plaintext_data"];
    if (j.find("iv_data") != j.end()) session.iv_data = j["iv_data"];
    if (j.find("ciphertext_data") != j.end()) session.ciphertext_data = j["ciphertext_data"];
    if (j.find("gcm_authentication_tag_data") != j.end()) session.gcm_authentication_tag_data = j["gcm_authentication_tag_data"];

    return session;
}