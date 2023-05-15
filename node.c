#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
//Code based on Assignment 7 doc and my node.c from Assignment 5
//Node already defined and struct created in node.h
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        //Conditionals from Eric's Section on December 1st
        //Checks if oldspeak or newspeak are null before setting accordingly
        n->oldspeak = (oldspeak == NULL ? NULL : strdup(oldspeak));
        n->newspeak = (newspeak == NULL ? NULL : strdup(newspeak));
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}

//Code based on assignment 5 node.c
void node_delete(Node **n) {
    if (*n) {
        //freeing oldspeak and newspeak
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
            (*n)->oldspeak = NULL;
        }
        if ((*n)->newspeak) {
            free((*n)->newspeak);
            (*n)->newspeak = NULL;
        }
        free(*n);
        *n = NULL;
    }
}

//Code based on Assignment 7 PDF
void node_print(Node *n) {
    // printf("node_print called successfully\n");
    if (n) {
        //printf("If n exists in node_print()\n");
        //wanted to do a switch statement, but unsure how it would work here
        if (n->oldspeak && n->newspeak) {
            //printf("Oldspeak has newspeak translation\n");
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        }
        if (n->oldspeak && !(n->newspeak)) {
            ///printf("Oldspeak has no translation\n");
            printf("%s\n", n->oldspeak);
        }
    }
}
