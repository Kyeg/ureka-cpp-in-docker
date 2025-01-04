#include "msg_generator_u_ticket.hpp" 
#include <string>
#include <iostream>
#include <cstdlib>

#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"

// #include "../../model/data_model/this_device.hpp"
// #include "../../model/data_model/this_person.hpp"
// #include "../../model/data_model/other_device.hpp"
#include "../../resource/crypto/ecdh.hpp"
#include "../../resource/crypto/ecc.hpp"

using namespace std;

UTicket UTicketGenerator::generate_arbitrary_u_ticket(string arbitrary_dict) {
    string success_msg = "-> SUCCESS: GENERATE_UITICKET";
    string failure_msg = "-> FAILURE: GENERATE_UITICKET";
    /*####################################################
    # Unsigned UTicket
    ####################################################*/

    //TODO, something UTicket(**arbitrary_dict) was written
    UTicket new_u_ticket = uticket_from_json_str(arbitrary_dict);

    if (new_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET) {
        new_u_ticket.ticket_order = 0;
    } else {
        new_u_ticket.ticket_order = device_table[new_u_ticket.device_id].ticket_order;
    }

    //cout << "info: " << "new_u_ticket = " << new_u_ticket.to_json_str() << endl;

    //Generate UTicket Id (Hash-based)
    new_u_ticket.u_ticket_id = generate_sha256_hash_bytes(new_u_ticket.to_json_str());

    /*####################################################
    # Signed UTicket
    ######################################################
    # Generate Signature

    # "device"*/

    if (
        new_u_ticket.u_ticket_type == u_ticket::TYPE_INITIALIZATION_UTICKET
            || new_u_ticket.u_ticket_type == u_ticket::TYPE_SELFACCESS_UTICKET
            || new_u_ticket.u_ticket_type == u_ticket::TYPE_CMD_UTOKEN
            || new_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_END_UTOKEN
    ) {
        //no issuer signature
        cout << "info: " << success_msg << endl;
    } else if (new_u_ticket.u_ticket_type == u_ticket::TYPE_OWNERSHIP_UTICKET
            || new_u_ticket.u_ticket_type == u_ticket::TYPE_ACCESS_UTICKET) {
        new_u_ticket = self_add_issuer_signature_on_u_ticket(new_u_ticket, this_person.strings_to_pp_keys());
        //log ("info", success_msg);
        cout << "info: " << success_msg << endl;
    } else {
        //log ("error", failure_msg);
        cout << "error: " << failure_msg << endl;
        exit(1);
    }

    //cout << "info: " << "new_u_ticket = " << new_u_ticket.to_json_str() << endl;

    return new_u_ticket;

}

UTicket UTicketGenerator::self_add_issuer_signature_on_u_ticket(UTicket unsigned_u_ticket, mbedtls_ecdsa_context key) {

    // Generate Signature
    size_t sig_len;
    unsigned char* signature = sign_message(unsigned_u_ticket.to_json_str().c_str(), key, sig_len);

    // Convert signature to string
    string new_sig = byte_to_hex(signature, sig_len);
    // Add Signature
    unsigned_u_ticket.issuer_signature = new_sig;

    return unsigned_u_ticket;
}