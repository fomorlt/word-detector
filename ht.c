#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "bst.h"
#include "ht.h"
#include "node.h"
#include "speck.h"
#include "salts.h"

//Code based on Assignment 7 document and Eric's Section on November 24th
uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

//Code based on Eric's section on November 24th
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(size * sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        ht->size = size;
    }
    return ht;
}

void ht_print(HashTable *ht) {
    //probably use a node print here
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        node_print(ht->trees[i]); //gotta do some address/pointer stuff?
    }
}

//Code based on Eric's section on November 24th
void ht_delete(HashTable **ht) {
    //go through tree, delete all nodes
    if (*ht) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            if ((*ht)->trees[i] != NULL) {
                bst_delete(&((*ht)->trees[i]));
            }
        }
        free((*ht)->trees);
        (*ht)->trees = NULL;
        free(*ht);
        *ht = NULL;
    }
}

uint32_t ht_size(HashTable *ht) {
    //self-explanatory; returns the size value of the hash table
    return ht->size;
}

//Code based on Eric's section on November 24th
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    //rehash oldspeak, and modulo in
    //order to bound it to the size of the hash table.
    //bst_find will return the node that
    //contains the oldspeak.
    lookups++;
    uint32_t i = (hash(ht->salt, oldspeak) % ht->size);
    return bst_find(ht->trees[i], oldspeak);
}

//Code based on Eric's section on November 24th
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    //hash the oldspeak to find index to store oldspeak in
    //bst_insert will insert the data into the tree at
    //the proper index
    lookups++;
    if (ht && oldspeak) {
        uint32_t i = (hash(ht->salt, oldspeak) % ht->size);
        ht->trees[i] = bst_insert(ht->trees[i], oldspeak, newspeak);
    }
}

//Code based on Eric's section on November 24th
uint32_t ht_count(HashTable *ht) {
    //iterate through the tree which contains every node
    //so we can just count everything that exists (not NULL)
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            count++;
        }
    }
    return count;
}

//Code based on Eric's section on November 24th
double ht_avg_bst_size(HashTable *ht) {
    //get the size of every binary search tree
    //return the average aka sum of sizes divided by number of
    //entries
    uint32_t avgcount = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avgcount += bst_size(ht->trees[i]);
    }
    //cast everything as doubles so we can get decimals
    double avgcountdouble = (double) avgcount;
    double htcountdouble = (double) ht_count(ht);
    return avgcountdouble / htcountdouble;
}

//Code based on Eric's section on November 24th
double ht_avg_bst_height(HashTable *ht) {
    //Same logic as bst size - get height of every bst
    //return sum of heights divided by number of entries
    uint32_t avg = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avg += bst_height(ht->trees[i]);
    }
    //cast everything as doubles so we can get decimals
    double avgdouble = (double) avg;
    double htcountdouble = (double) ht_count(ht);
    return avgdouble / htcountdouble;
}
