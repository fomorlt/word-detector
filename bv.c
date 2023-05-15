#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bv.h"

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

//Code based on Professor Long's bv8.c in the code comments repository
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        //ternary operator adds a 1 to account for an extra byte
        uint32_t bytes = length / 8 + (length % 8 ? 1 : 0); //turns length to number of bytes
        bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        //allocating the vector itself, initializing to 0 with calloc
        return bv;
    } else {
        return NULL;
    }
}

//Code based on Professor Long's bv8.c in the code comments repository
void bv_delete(BitVector **bv) {
    if (*bv) {
        if ((*bv)->vector) {
            free((*bv)->vector);
            (*bv)->vector = NULL;
        }
        free(*bv);
        *bv = NULL;
    }
}

uint32_t bv_length(BitVector *bv) {
    if (bv) {
        return bv->length;
    }
    return 0;
}

//Code based on Professor Long's bv8.c in the code comments repository and my code.c from assignment 5
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if ((bv && bv->vector) && (i <= bv->length)) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        // i / 8 to get to byte, then within byte count up from modulo
        //bit shifting, using or with 0x1 as a 'hole punch' of sorts to set the array
        //at desired index to 1
        return true;
    }
    return false;
}

//Code based on Professor Long's bv8.c in the code comments repository and my code.c from assignment 5
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if ((bv && bv->vector) && (i <= bv->length)) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        //using an inverted 0x1, aka something like 1111110111111
        //to hole punch a 0 into the vector array
        //Same idea as set_bit
        return true;
    }
    return false;
}

//Code based on Professor Long's bv8.c in the code comments repository and my code.c from assignment 5
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if ((bv && bv->vector) && (i <= bv->length)) {
        if (((bv->vector[i / 8] >> (i % 8)) & 0x1) == 1) {
            //access the byte, shift the bit to the edge
            //and & it with 1 to check if 1: if so, return true, else return false
            return true;
        }
        return false; //probably unecessary
    }
    return false;
}

//Self-created
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        printf("%d", bv->vector[i]);
    }
}
