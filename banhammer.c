#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>

#include "bv.h"
#include "bf.h"
#include "ht.h"
#include "parser.h"
#include "bst.h"
#include "salts.h"
#include "node.h"
#include "messages.h"

#define OPTIONS     "ht:f:s"
#define REG_PATTERN "[A-Za-z0-9_]+(('|-)[A-Za-z0-9_]+)*"

//Regex pattern and main code based on Eric's section on December 1st and the Assignment 7 PDF
int main(int argc, char **argv) {
    int opt = 0;

    //cases
    bool stats = false;
    bool help = false;

    //defaults
    uint32_t bloomsize = 1048576;
    uint32_t hashsize = 65536;

    //switch statement
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            //help wanted!
            help = true;
            break;
        case 't':
            //user specifies number of hash table entries
            hashsize = strtoul(optarg, NULL, 10);
            break;
        case 'f':
            //user specifies number of bloom filter entries
            bloomsize = strtoul(optarg, NULL, 10);
            break;
        case 's':
            //user wants stats printed
            stats = true;
            break;
        }
    }
    if (help) {
        printf("SYNOPSIS\n"
               "A word filtering program for the GPRSC.\n"
               "Filters out and reports bad words parsed from stdin.\n"
               "\n"
               "USAGE\n"
               "  ./banhammer [-hs] [-t size] [-f size]\n"
               "\n"
               "OPTIONS\n"
               "  -h           Program usage and help.\n"
               "  -s           Print program statistics.\n"
               "  -t size      Specify hash table size (default: 2^16).\n"
               "  -f size      Specify Bloom filter size (default: 2^20).\n");
        return 0;
    }

    //Check if sizes are valid
    if ((bloomsize <= 0)) {
        fprintf(stderr, "Sorry, your bloom size can't be negative or 0!");
        return EXIT_FAILURE;
    }
    if (hashsize <= 0) {
        fprintf(stderr, "Sorry, your hash size can't be negative or 0!");
        return EXIT_FAILURE;
    }

    //creating the bloom filter and hash table
    BloomFilter *bf = bf_create(bloomsize);
    HashTable *ht = ht_create(hashsize);

    //add badspeak
    char badspeak[1024];
    //buffer to hold badspeak read
    FILE *badspeaktxt = fopen("badspeak.txt", "r");
    while (fscanf(badspeaktxt, "%s\n", badspeak) != EOF) {
        //reads in words from badspeak.txt until end of file
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL); //badspeak is oldspeak without newspeak translation
    }
    fclose(badspeaktxt);

    //add newspeak translations
    //works the same as adding badspeak  function
    char oldspeak[1024], newspeak[1024];
    FILE *newspeaktxt = fopen("newspeak.txt", "r");
    while (fscanf(newspeaktxt, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }

    fclose(newspeaktxt);

    //Regex implementation
    regex_t reg;
    if (regcomp(&reg, REG_PATTERN, REG_EXTENDED) != 0) {
        //stores regex patterm defined in REG_PATTERN macro to reg
        //function will return 0 if success - so if not, error!
        fprintf(stderr, "Sorry, unable to save regex!");
        return EXIT_FAILURE;
    }

    char *word = NULL;
    //Creating binary search trees to store violations
    Node *mixedmsg = bst_create();
    Node *badmsg = bst_create();
    while ((word = next_word(stdin, &reg))) {
        //reading from stdin
        //for loop to turn word lowercase
        for (uint32_t i = 0; i < strlen(word); i++) {
            //turn the word lowercase
            word[i] = tolower(word[i]);
        }

        if (bf_probe(bf, word)) {
            //if in bloom filter, is either mixedspeak or badspeak
            Node *n = ht_lookup(ht, word);
            if ((n != NULL) && (n->newspeak != NULL)) {
                //if the node has a newspeak translation
                mixedmsg = bst_insert(mixedmsg, n->oldspeak, n->newspeak);
            } else if ((n != NULL) && (n->newspeak == NULL)) {
                //badspeak - no newspeak translation
                badmsg = bst_insert(badmsg, n->oldspeak, n->newspeak);
            }
        }
    }

    //Statistics implementation according to assignment 7 PDF
    //Cast all as double so we get decimal values
    if (stats) {
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        double avgtraverse = ((double) branches / (double) lookups);
        printf("Average branches traversed: %.6f\n", avgtraverse);
        double htload = 100 * ((double) ht_count(ht) / (double) ht_size(ht));
        double bfload = 100 * ((double) bf_count(bf) / (double) bf_size(bf));
        printf("Hash table load: %.6f%%\n", htload);
        printf("Bloom filter load: %.6f%%\n", bfload);
        //Return 0 here to end program and suppress all other messages
        return 0;
    } else {
        //Testing which type of crime has been commited - mixedspeak, a mix both, or complete badspeak
        if ((bst_size(mixedmsg) > 0) && (bst_size(badmsg) == 0)) {
            //Mixed message transgression
            //Goodspeak message
            printf("%s", goodspeak_message);
            bst_print(mixedmsg);
        } else if ((bst_size(mixedmsg) > 0) && (bst_size(badmsg) > 0)) {
            //Both oldspeak with translations and badspeak found
            printf("%s", mixspeak_message);
            bst_print(badmsg);
            bst_print(mixedmsg);
        } else if ((bst_size(mixedmsg) == 0) && (bst_size(badmsg) > 0)) {
            //Only badspeak found
            printf("%s", badspeak_message);
            bst_print(badmsg);
        }
    }

    //Cleanup
    //Free a bunch of stuff
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&badmsg);
    bst_delete(&mixedmsg);
    regfree(&reg);
    branches = 0;
    lookups = 0;
    return 0;
}
