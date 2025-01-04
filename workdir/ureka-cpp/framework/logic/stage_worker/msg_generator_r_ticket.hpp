#ifndef MSG_GENERATOR_R_TICKET_HPP
#define MSG_GENERATOR_R_TICKET_HPP
#include "../../model/data_model/this_device.hpp"
#include "../../model/data_model/this_person.hpp"
#include "../../model/data_model/other_device.hpp"
#include "../../model/message_model/r_ticket.hpp"

#include "../../resource/crypto/ecdh.hpp"
#include "../../resource/crypto/ecc.hpp"

#include <string>
#include <map>

using namespace std;

class RTicketGenerator{
public:
    ThisDevice this_device;
    ThisPerson this_person;
    map<string, OtherDevice> device_table;



    RTicketGenerator(ThisDevice this_device, ThisPerson this_person, map<string, OtherDevice> device_table) : this_device(this_device), this_person(this_person), device_table(device_table) {}

    RTicket generate_arbitrary_r_ticket(string arbitrary_dict);

    RTicket self_add_device_signature_on_r_ticket(RTicket unsigned_r_ticket, mbedtls_ecdsa_context key);

};

#endif