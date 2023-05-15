#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"

//Code based off of assignment 7 document and Eric's section on November 24th, and December 1
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};
//each salt has 2 values

//Code based on Eric's section on November 24th
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        //size is size of filter
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
    }
    return bf;
}

//Code based on Eric's section on the 24th
void bf_delete(BloomFilter **bf) {
    if (*bf) {
        //bloom filter made of bit vectors, so clear those
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

//Code based
uint32_t bf_size(BloomFilter *bf) {
    //Size of the bloom filter is length of it's bit vector, as per assignment 7 PDF
    return bv_length(bf->filter);
}

//Code based on Eric's section on November 24th and Assignment 7 PDF
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t i, j, k;
    i = hash(bf->primary, oldspeak) % bf_size(bf);
    j = hash(bf->secondary, oldspeak) % bf_size(bf);
    k = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, i);
    bv_set_bit(bf->filter, j);
    bv_set_bit(bf->filter, k);
}

//Code based on Eric's section on November 24th and Assignment 7 PDF
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t i, j, k;
    i = hash(bf->primary, oldspeak) % bf_size(bf);
    j = hash(bf->secondary, oldspeak) % bf_size(bf);
    k = hash(bf->tertiary, oldspeak) % bf_size(bf);
    if ((bv_get_bit(bf->filter, i)) && (bv_get_bit(bf->filter, j)) && (bv_get_bit(bf->filter, k))) {
        return true;
    } else {
        return false;
    }
}

//Code based on Eric's Section on November 24th
uint32_t bf_count(BloomFilter *bf) {
    uint32_t counter = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); i++) {
        counter += bv_get_bit(bf->filter, i);
    }
    return counter;
}

//Code based on Assignment 7 PDF
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
