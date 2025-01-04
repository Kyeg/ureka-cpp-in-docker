#ifndef MSG_GENERATOR_U_TICKET_HPP
#define MSG_GENERATOR_U_TICKET_HPP
#include "../../model/data_model/this_device.hpp"
#include "../../model/data_model/this_person.hpp"
#include "../../model/data_model/other_device.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../model/message_model/r_ticket.hpp"

#include "mbedtls/ecdsa.h"
#include "../../resource/crypto/ecdh.hpp"
#include "../../resource/crypto/ecc.hpp"
#include <string>
#include <map>

using namespace std;

class UTicketGenerator{
private:
    ThisDevice this_device;
    ThisPerson this_person;
    map<string, OtherDevice> device_table;

public:

UTicketGenerator(ThisDevice this_device, ThisPerson this_person, map<string, OtherDevice> device_table){
    this->this_device = this_device;
    this->this_person = this_person;
    this->device_table = device_table;
}

UTicket generate_arbitrary_u_ticket(string arbitrary_dict);

UTicket self_add_issuer_signature_on_u_ticket(UTicket unsigned_r_ticket, mbedtls_ecdsa_context key);

};

#endif