#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/platform.h"

#if defined(MBEDTLS_ECDSA_C) && \
    defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"

#include <string.h>
#endif
#include "ecc.hpp"
#include <iostream>

#include <mbedtls/pk.h>
#include <mbedtls/pem.h>
#include <mbedtls/error.h>

static void dump_buf(const char *title, unsigned char *buf, size_t len)
{
    size_t i;

    mbedtls_printf("%s", title);
    for (i = 0; i < len; i++)
    {
        std::cout << std::hex << (int)buf[i];
    }
    mbedtls_printf("\n");
}

static void dump_pubkey(const char *title, mbedtls_ecdsa_context *key)
{
    unsigned char buf[300];
    size_t len;

    if (mbedtls_ecp_point_write_binary(&key->grp, &key->Q,
                                       MBEDTLS_ECP_PF_UNCOMPRESSED, &len, buf, sizeof(buf)) != 0)
    {
        mbedtls_printf("internal error\n");
        return;
    }

    // dump_buf(title, buf, len);
}

mbedtls_ecdsa_context generate_key_pair()
{
    mbedtls_ecdsa_context ctx;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "ecdsa";
    int ret = 1;

    mbedtls_ecdsa_init(&ctx);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_entropy_init(&entropy);

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
    {
        std::cout << "Failed to seed random number generator" << std::endl;
    }

    // if (mbedtls_ecdsa_genkey(&ctx, MBEDTLS_ECP_DP_SECP256K1, mbedtls_ctr_drbg_random, &ctr_drbg) != 0)
    // {
    //     std::cout << "Failed to generate key pair" << std::endl;
    // }

    // use ecp gen key instead of ecdsa gen key
    if (mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256K1, &ctx, mbedtls_ctr_drbg_random, &ctr_drbg) != 0)
    {
        std::cout << "Failed to generate key pair" << std::endl;
    }
    // mbedtls_printf(" ok (key size: %d bits)\n", (int)ctx.grp.pbits);
    // dump_pubkey("Public key: ", &ctx);

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ctx;
}

unsigned char *sign_message(const char *message, mbedtls_ecdsa_context key, size_t &global_sig_len)
{
    mbedtls_ecdsa_context ctx = key;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "ecdsa";
    unsigned char hash[32];
    unsigned char *signature;
    size_t sig_len;
    int ret;

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);


    //give ctx private key
    // ctx.grp = key.grp;
    // ctx.d = key.d;

    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, sizeof(pers) - 1);

    if ((ret = mbedtls_sha256_ret((const unsigned char *)message, strlen(message), hash, 0)) != 0)
    {
        std::cout << "Failed to hash message" << std::endl;
    }

    // output the hash
    // std::cout << "Hash: ";
    // for (int i = 0; i < 32; i++)
    //     std::cout << std::hex << (int)hash[i];
    // std::cout << std::endl;

    signature = (unsigned char *)malloc(65);

    mbedtls_ecdsa_write_signature(&ctx, MBEDTLS_MD_SHA256, hash, sizeof(hash), signature, &sig_len, mbedtls_ctr_drbg_random, &ctr_drbg);

    // output the signature
    // std::cout << "Signature: " << sig_len << " bytes" << std::endl;
    global_sig_len = sig_len;
    // for (int i = 0; i < 65; i++)
    //     std::cout << std::hex << (int)signature[i];
    // std::cout << std::endl;
    // mbedtls_ecdsa_free(&ctx);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return signature;
}

int verify_signature(const char *message, const unsigned char *signature, mbedtls_ecdsa_context ctx, size_t sig_len)
{
    // mbedtls_entropy_context entropy;
    // mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "ecdsa";
    unsigned char hash[32];
    int ret;

    // mbedtls_entropy_init(&entropy);
    // mbedtls_ctr_drbg_init(&ctr_drbg);

    // mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, sizeof(pers) - 1);

    if ((ret = mbedtls_sha256_ret((const unsigned char *)message, strlen(message), hash, 0)) != 0)
    {
        std::cout << "Failed to hash message" << std::endl;
    }

    if (mbedtls_ecdsa_read_signature(&ctx, hash, 32, (const unsigned char *)signature, sig_len) != 0)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    mbedtls_ecdsa_free(&ctx);
    // mbedtls_ctr_drbg_free(&ctr_drbg);
    // mbedtls_entropy_free(&entropy);

    return ret;
}

// int main()
// {
//     mbedtls_ecdsa_context key = generate_key_pair();
//     size_t sig_len;
//     string private_key = extractPrivateKeyToString(key);
//     unsigned char *signature = sign_message("hello", key, sig_len, private_key);
//     int ret = verify_signature("hello", signature, key, sig_len);
//     free(signature);
//     std::cout << "Signature verification: " << ret << std::endl;
//     return 0;
// }
