#ifndef RECEIVED_MSG_STORER_HPP
#define RECEIVED_MSG_STORER_HPP

#include "../../model/shared_data.hpp"
#include "../../model/message_model/u_ticket.hpp"
#include "../../model/message_model/r_ticket.hpp"

#include "../../resource/storage/simple_storage.hpp"


class ReceivedMsgStorer {
public:
    ReceivedMsgStorer(SharedData* shared_data, 
                      SimpleStorage* simple_storage);

    void store_received_xxx_u_ticket(const UTicket& received_u_ticket);
    void store_received_xxx_r_ticket(const RTicket& received_r_ticket);

private:
    SharedData* shared_data;
    // MeasureHelper measure_helper;
    SimpleStorage* simple_storage;
};

#endif // RECEIVED_MSG_STORER_HPP
