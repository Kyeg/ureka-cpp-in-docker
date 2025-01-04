#ifndef GENERATED_MSG_STORER_HPP
#define GENERATED_MSG_STORER_HPP

#include <string>
#include "../../model/shared_data.hpp"
// #include "../../resource/storage/simple_storage.hpp"
#include "../../model/message_model/u_ticket.hpp"

class GeneratedMsgStorer {
public:
    GeneratedMsgStorer(SharedData* shared_data/*, SimpleStorage simple_storage*/) : shared_data(shared_data){};

    void store_generated_xxx_u_ticket(const std::string generated_u_ticket_json);

private:
    SharedData* shared_data;
    // SimpleStorage simple_storage;
};

#endif // GENERATED_MSG_STORER_HPP