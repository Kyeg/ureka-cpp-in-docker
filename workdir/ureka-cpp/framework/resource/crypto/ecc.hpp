#ifndef ECC_H
#define ECC_H

#include "mbedtls/ecdsa.h"

// generate a new key pair
mbedtls_ecdsa_context generate_key_pair();

// sign a message
unsigned char *sign_message(const char *message, mbedtls_ecdsa_context key, size_t &global_sig_len);   

// verify a signature
int verify_signature(const char *message, const unsigned char *signature, mbedtls_ecdsa_context key, size_t sig_len);
#endif