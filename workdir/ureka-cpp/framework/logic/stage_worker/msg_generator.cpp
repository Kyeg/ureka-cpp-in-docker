#include "msg_generator.hpp"
#include <string>
#include <iostream>

using namespace std;

string MsgGenerator::self_generate_xxx_u_ticket(string arbitrary_dict){
    cout << "info" << shared_data->this_device.device_name << " is generating u_ticket" << endl;

    UTicketGenerator u_ticket_generator = UTicketGenerator(
        shared_data->this_device,
        shared_data->this_person,
        shared_data->device_table
    );
    
    UTicket generated_u_ticket = u_ticket_generator.generate_arbitrary_u_ticket(arbitrary_dict);


    return generated_u_ticket.to_json_str();
}

string MsgGenerator::self_generate_xxx_r_ticket(string arbitrary_dict){
    cout << "info " << shared_data->this_device.device_name << " is generating r_ticket" << endl;
    // cout << "info  come here" << shared_data->this_device.to_json() << endl;
    RTicketGenerator r_ticket_generator = RTicketGenerator(
        shared_data->this_device,
        shared_data->this_person,
        shared_data->device_table
    );

    // cout << "info  come here" << r_ticket_generator.this_device.to_json() << endl;

    RTicket generated_r_ticket = r_ticket_generator.generate_arbitrary_r_ticket(arbitrary_dict);


    return generated_r_ticket.to_json_str();
}