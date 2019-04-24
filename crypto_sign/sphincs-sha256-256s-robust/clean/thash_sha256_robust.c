#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "sha2.h"
#include "sha256.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
static void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    unsigned char *bitmask = buf + SPX_N + SPX_SHA256_ADDR_BYTES;
    uint8_t sha2_state[40];
    unsigned int i;

    memcpy(buf, pub_seed, SPX_N);
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_compress_address(buf + SPX_N, addr);
    /* MGF1 requires us to have 4 extra bytes in 'buf' */
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_mgf1(bitmask, inblocks * SPX_N, buf, SPX_N + SPX_SHA256_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    memcpy(sha2_state, PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_state_seeded, 40 * sizeof(uint8_t));

    for (i = 0; i < inblocks * SPX_N; i++) {
        buf[SPX_N + SPX_SHA256_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sha256_inc_finalize(outbuf, sha2_state, buf + SPX_N,
                        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N);
    memcpy(out, outbuf, SPX_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 1 * SPX_N + 4];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, 1, pub_seed, addr);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 2 * SPX_N + 4];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, 2, pub_seed, addr);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + SPX_WOTS_LEN * SPX_N + 4];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, SPX_WOTS_LEN, pub_seed, addr);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + SPX_FORS_TREES * SPX_N + 4];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, SPX_FORS_TREES, pub_seed, addr);
}
