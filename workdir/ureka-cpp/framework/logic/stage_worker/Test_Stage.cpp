#include "msg_generator_r_ticket.hpp"
#include "msg_generator_u_ticket.hpp"
#include "msg_verifier_r_ticket.hpp"
#include "msg_verifier_u_ticket.hpp"
#include "msg_verifier_message.hpp"
#include "msg_verifier.hpp"
#include "msg_generator.hpp"
#include "generated_msg_storer.hpp"
#include "executor.hpp"
#include "../../resource/crypto/ecdh.hpp"
#include "../../resource/crypto/ecc.hpp"
#include "../../model/shared_data.hpp"
#include "mbedtls/ecdsa.h"
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"
#include "../../resource/crypto/ecdh.hpp"

#include <iostream>
#include <string>

using namespace std;
using json = nlohmann::json;

// RTicketGenerator test=======================================================

TEST(RTicketGeneratorTest, SelfAddDeviceSignatureOnRTicketTest) {
    ThisDevice this_device;
    ThisPerson this_person;
    map<string, OtherDevice> device_table;
    RTicketGenerator generator(this_device, this_person, device_table);

    RTicket unsigned_r_ticket;
    unsigned_r_ticket.r_ticket_id = "123456";
    unsigned_r_ticket.r_ticket_type = r_ticket::TYPE_CRKE1_RTICKET;
    unsigned_r_ticket.device_id = "device123";

    mbedtls_ecdsa_context keys = generate_key_pair();
    string public_key = pub_key_to_string(keys);


    RTicket signed_r_ticket = generator.self_add_device_signature_on_r_ticket(unsigned_r_ticket, keys);

    string byte_sig = hex_to_byte(signed_r_ticket.device_signature);
    size_t signature_len = byte_sig.length();
    unsigned char signature[signature_len];
    for (size_t i = 0; i < signature_len; i++) {
        signature[i] = static_cast<unsigned char>(byte_sig[i]);
    }

    int verified = verify_signature(unsigned_r_ticket.to_json_str().c_str(), signature, keys, signature_len);

    // EXPECT_EQ(signed_r_ticket.device_signature, signature);
    EXPECT_EQ(verified, 1);

}

// ============================================================================

// UTicketGenerator test=======================================================

TEST(UTicketGeneratorTest, SelfAddIssuerSignatureOnUTicketTest) {
    ThisDevice this_device;
    ThisPerson this_person;
    map<string, OtherDevice> device_table;
    UTicketGenerator generator(this_device, this_person, device_table);

    UTicket unsigned_u_ticket;
    unsigned_u_ticket.u_ticket_id = "123456";
    unsigned_u_ticket.u_ticket_type = u_ticket::TYPE_OWNERSHIP_UTICKET;
    unsigned_u_ticket.device_id = "device123";

    mbedtls_ecdsa_context keys = generate_key_pair();
    // string private_key = pk_key_to_string(keys);

    UTicket signed_u_ticket = generator.self_add_issuer_signature_on_u_ticket(unsigned_u_ticket, keys);


    string byte_sig = hex_to_byte(signed_u_ticket.issuer_signature);
    size_t signature_len = byte_sig.length();
    unsigned char *signature = (unsigned char*)byte_sig.c_str();

    int verified = verify_signature(unsigned_u_ticket.to_json_str().c_str(), signature, keys, signature_len);

    // EXPECT_EQ(signed_r_ticket.device_signature, signature);
    EXPECT_EQ(verified, 1);

}

// ============================================================================

// MsgVerify_RTicket test===========================================================

TEST(MsgVerifyRTicketTest, VerifyJsonSchema) {
    ThisDevice this_device;
    UTicket audit_start_ticket;
    UTicket audit_end_ticket;
    map<string, OtherDevice> device_table;
    CurrentSession current_session;
    RTicketVerifier verifier(this_device, audit_start_ticket, audit_end_ticket, device_table, current_session);

    RTicket r_ticket;
    r_ticket.r_ticket_id = "123456";
    r_ticket.r_ticket_type = r_ticket::TYPE_CRKE1_RTICKET;
    r_ticket.device_id = "device123";

    RTicket verified_r_ticket = verifier.verify_json_schema(r_ticket.to_json_str());

    EXPECT_EQ(verified_r_ticket.r_ticket_id, r_ticket.r_ticket_id);
    EXPECT_EQ(verified_r_ticket.r_ticket_type, r_ticket.r_ticket_type);
    EXPECT_EQ(verified_r_ticket.device_id, r_ticket.device_id);
}

TEST(MsgVerifyRTicketTest, VerifyProtocolVersion) {
    ThisDevice this_device;
    UTicket audit_start_ticket;
    UTicket audit_end_ticket;
    map<string, OtherDevice> device_table;
    CurrentSession current_session;
    RTicketVerifier verifier(this_device, audit_start_ticket, audit_end_ticket, device_table, current_session);

    RTicket r_ticket;
    r_ticket.protocol_version = "UREKA-1.0";

    RTicket verified_r_ticket = verifier.verify_protocol_version(r_ticket);

    EXPECT_EQ(verified_r_ticket.protocol_version, r_ticket.protocol_version);
}

TEST(MsgVerifyRTicketTest, SelfVerifyDeviceSignatureOnRTicketTest) {
    ThisDevice this_device;
    ThisPerson this_person;
    UTicket audit_start_ticket;
    UTicket audit_end_ticket;
    map<string, OtherDevice> device_table;
    CurrentSession current_session;
    RTicketVerifier verifier(this_device, audit_start_ticket, audit_end_ticket, device_table, current_session);

    RTicket signed_r_ticket;
    signed_r_ticket.r_ticket_id = "123456";
    signed_r_ticket.r_ticket_type = r_ticket::TYPE_CRKE1_RTICKET;
    signed_r_ticket.device_id = "device123";

    mbedtls_ecdsa_context keys = generate_key_pair();
    string public_key = pub_key_to_string(keys);

    // crate generator
    RTicketGenerator generator(this_device, this_person, device_table);

    RTicket signed_r_ticket2 = generator.self_add_device_signature_on_r_ticket(signed_r_ticket, keys);

    bool verified = verifier.self_verify_device_signature_on_r_ticket(signed_r_ticket2, public_key);

    EXPECT_EQ(verified, true);

}

// ============================================================================

// MsgVerify_UTicket test===========================================================

TEST(MsgVerifyUTicketTest, VerifyJsonSchema) {
    ThisDevice this_device;
    UTicketVerifier verifier(this_device);

    UTicket u_ticket;
    u_ticket.u_ticket_id = "123456";
    u_ticket.u_ticket_type = u_ticket::TYPE_OWNERSHIP_UTICKET;
    u_ticket.device_id = "device123";

    UTicket verified_u_ticket = verifier.verify_json_schema(u_ticket.to_json_str());

    EXPECT_EQ(verified_u_ticket.u_ticket_id, u_ticket.u_ticket_id);
    EXPECT_EQ(verified_u_ticket.u_ticket_type, u_ticket.u_ticket_type);
    EXPECT_EQ(verified_u_ticket.device_id, u_ticket.device_id);
}

TEST(MsgVerifyUTicketTest, SelfVerifyIssuerSignatureOnUTicket) {
    ThisDevice this_device;
    UTicketVerifier verifier(this_device);

    UTicket signed_u_ticket;
    signed_u_ticket.u_ticket_id = "123456";
    signed_u_ticket.u_ticket_type = u_ticket::TYPE_OWNERSHIP_UTICKET;
    signed_u_ticket.device_id = "device123";

    mbedtls_ecdsa_context keys = generate_key_pair();
    string public_key = pub_key_to_string(keys);

    // crate generator
    ThisPerson this_person;
    map<string, OtherDevice> device_table;
    UTicketGenerator generator(this_device, this_person, device_table);

    UTicket signed_u_ticket2 = generator.self_add_issuer_signature_on_u_ticket(signed_u_ticket, keys);

    bool verified = verifier.self_verify_issuer_signature_on_u_ticket(signed_u_ticket2, public_key);

    EXPECT_EQ(verified, true);

}

// ============================================================================

// MsgVerify_Message test===========================================================

TEST(MsgVerifyMessageTest, VerifyJsonSchema) {
    ThisDevice this_device;
    MessageVerifier verifier(this_device);

    Message message;
    message.message_type = "RTICKET";
    message.message_operation = message::MESSAGE_RECV_AND_STORE;
    message.message_str = "{'r_ticket_id': '123456', 'r_ticket_type': 'CRKE1_RTICKET', 'device_id': 'device123'}";

    Message verified_message = verifier.verify_json_schema(message.to_json_str());

    EXPECT_EQ(verified_message.message_type, message.message_type);
    EXPECT_EQ(verified_message.message_operation, message.message_operation);
    EXPECT_EQ(verified_message.message_str, message.message_str);
}

TEST(MsgVerifyMessageTest, VerifyMessageType) {
    ThisDevice this_device;
    MessageVerifier verifier(this_device);

    Message message;
    message.message_type = "RTICKET";
    message.message_operation = message::MESSAGE_RECV_AND_STORE;
    message.message_str = "{'r_ticket_id': '123456', 'r_ticket_type': 'CRKE1_RTICKET', 'device_id': 'device123'}";

    Message verified_message = verifier.verify_message_type(message);

    EXPECT_EQ(verified_message.message_type, message.message_type);
}

TEST(MsgVerifyMessageTest, VerifyMessageOperation) {
    ThisDevice this_device;
    MessageVerifier verifier(this_device);

    Message message;
    message.message_type = "RTICKET";
    message.message_operation = message::MESSAGE_RECV_AND_STORE;
    message.message_str = "{'r_ticket_id': '123456', 'r_ticket_type': 'CRKE1_RTICKET', 'device_id': 'device123'}";

    Message verified_message = verifier.verify_message_operation(message);

    EXPECT_EQ(verified_message.message_operation, message.message_operation);
}

// ============================================================================

// MsgVerify test===========================================================

TEST(MsgVerifyTest, SelfClassifyMessageIsDefinedType_r) {
    json r_ticket;
    r_ticket["r_ticket_id"] = "123456";
    r_ticket["r_ticket_type"] = u_ticket::TYPE_INITIALIZATION_UTICKET;
    r_ticket["device_id"] = "device123";
    r_ticket["protocol_version"] = "UREKA-1.0";

    json message;
    message["message_type"] = "RTICKET";
    message["message_operation"] = message::MESSAGE_RECV_AND_STORE;
    message["message_str"] = r_ticket.dump(4);

    string message_str = message.dump(4);

    SharedData shared_data;
    MsgVerifier verifier(shared_data);

    pair<UTicket, RTicket> classified_message = verifier.self_classify_message_is_defined_type(message_str);

    EXPECT_EQ(classified_message.first.u_ticket_id, "");
    EXPECT_EQ(classified_message.second.r_ticket_id, "123456");
    EXPECT_EQ(classified_message.second.r_ticket_type, u_ticket::TYPE_INITIALIZATION_UTICKET);
}

TEST(MsgVerifyTest, SelfClassifyMessageIsDefinedType_u) {
    json u_ticket;
    u_ticket["u_ticket_id"] = "123456";
    u_ticket["u_ticket_type"] = u_ticket::TYPE_INITIALIZATION_UTICKET;
    u_ticket["protocol_version"] = "UREKA-1.0";
    u_ticket["device_id"] = "no_id";

    string u_ticket_str = u_ticket.dump(4);

    SharedData shared_data;
    MsgVerifier verifier(shared_data);

    UTicket classified_u_ticket = verifier.self_classify_u_ticket_is_defined_type(u_ticket_str);

    EXPECT_EQ(classified_u_ticket.u_ticket_id, "123456");
    EXPECT_EQ(classified_u_ticket.u_ticket_type, u_ticket::TYPE_INITIALIZATION_UTICKET);
    EXPECT_EQ(classified_u_ticket.device_id, "no_id");
}

TEST(MsgVerifierTest, VerifyCmdIsInTaskScope) {
    CurrentSession current_session;
    json taskScope;
    taskScope["SAY-HELLO-1"] = "allow";
    current_session.current_task_scope = taskScope.dump(4);
    SharedData shared_data;
    shared_data.current_session = current_session;

    MsgVerifier verifier(shared_data);

    verifier.verify_cmd_is_in_task_scope("HELLO-1");
}

// ============================================================================

// Executor test===========================================================

TEST(ExecutorTest, SelfExecuteDataProcessing) {
    ThisDevice this_device;
    ThisPerson this_person;
    map<string, OtherDevice> device_table;
    CurrentSession current_session;
    SharedData shared_data;
    shared_data.this_device = this_device;
    shared_data.this_person = this_person;
    shared_data.device_table = device_table;
    shared_data.current_session = current_session;

    Executor executor(shared_data);

    string plaintext_cmd = "HELLO";
    string associated_plaintext_cmd = "HELLO-ASSOCIATED";

    string* data_processing_result = executor.self_execute_data_processing(plaintext_cmd, associated_plaintext_cmd);

    string plaintext_data = data_processing_result[0];
    string associated_plaintext_data = data_processing_result[1];

    EXPECT_EQ(plaintext_data, "DATA: HELLO");
    EXPECT_EQ(associated_plaintext_data, "DATA: HELLO-ASSOCIATED");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}