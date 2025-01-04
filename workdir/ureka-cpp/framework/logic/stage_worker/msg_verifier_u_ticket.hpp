#ifndef MSG_VERIFIER_U_TICKET_HPP
#define MSG_VERIFIER_U_TICKET_HPP
#include <string>
#include "mbedtls/ecdsa.h"
#include "../../model/data_model/this_device.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../resource/crypto/ecdh.hpp"
#include "nlohmann/json.hpp"

using namespace std;

class UTicketVerifier {
public:
    ThisDevice this_device;

    UTicketVerifier(ThisDevice this_device){
        this->this_device = this_device;
    };

    UTicket verify_json_schema(string arbitrary_json);

    UTicket verify_protocol_version(UTicket u_ticket_in);

    UTicket verify_u_ticket_id(UTicket u_ticket_in);

    UTicket verify_u_ticket_type(UTicket u_ticket_in);

    UTicket has_device_id(UTicket u_ticket_in);

    UTicket verify_device_id(UTicket u_ticket_in);

    UTicket verify_ticket_order(UTicket u_ticket_in);

    UTicket verify_holder_id(UTicket u_ticket_in);

    UTicket verify_task_scope(UTicket u_ticket_in);

    UTicket verify_ps(UTicket u_ticket_in);

    UTicket verify_issuer_signature(UTicket u_ticket_in);

    /*####################################################
    # Verify ECC Signature on UTicket
    ####################################################*/

    bool self_verify_issuer_signature_on_u_ticket(UTicket signed_u_ticket, string public_key);
};

#endif // MSG_VERIFIER_U_TICKET_HPP