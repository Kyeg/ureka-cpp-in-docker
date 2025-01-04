#ifndef THIS_PERSON_HPP
#define THIS_PERSON_HPP
#include "mbedtls/ecdsa.h"
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class ThisPerson {
public:

    ThisPerson() {};

    ThisPerson(const ThisPerson& this_person) : person_priv_key(this_person.person_priv_key), person_pub_key(this_person.person_pub_key) {}

    string person_priv_key;
    string person_pub_key;

    mbedtls_ecdsa_context strings_to_pp_keys();

    /*##############################################################################
    #                                < Person_obj >                                #
    #                                       | self-defined serilaization           #
    #                                       | (including ECC_Key_obj, bytes, etc.) #
    #                                       v                                      #
    #         < JSON_dict (Should be JSON serializable, i.e. native type) >        #
    #                                       |                                      #
    #                                       v                                      #
    #                       < JSON_str (Printable Characters) >                    #
    ##############################################################################*/

    string to_json();
};

ThisPerson json_to_this_person(json j);

#endif