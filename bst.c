#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

//ternary operator: return x if x is greater than y - if not, then return y.
//returns y if equal, because doesn't matter then.
#define max(x, y) (x > y ? x : y)

//Code based on Assignment 7, Professor Long's Lecture 18, and Eric's section on November 24th.
uint64_t branches = 0;

//Code based on Eric's section on November 24th
Node *bst_create(void) {
    return NULL;
}

void bst_delete(Node **root) {
    if ((*root)) {
        if ((*root)->left != NULL) {
            bst_delete(&(*root)->left);
        }
        if ((*root)->right != NULL) {
            bst_delete(&(*root)->right);
        }
        node_delete(&(*root));
        //recursively goes through each node
        //and deletes from the bottom up.
    }
}

//Code based on Eric's Section on November 24th
uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    }
    return max(bst_height(root->left), bst_height(root->right)) + 1;
    //recursively traverse the tree, keeping track of the longest path
    //"downward" so to speak.
}

//Code based on Eric's section on November 24th
uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    return bst_size(root->left) + bst_size(root->right) + 1;
    //recursively traverses tree to  count the nodes,
    //so just increment by 1 every time you cross a node and add
    //the sum of the other nodes
}

//Code based on Eric's section on November 24th
Node *bst_find(Node *root, char *oldspeak) {
    Node *curr = root;
    //hold in curr so we don't modify the the root in parameter
    //root exists and has words stored, word to find is not null
    if ((root != NULL) && (oldspeak != NULL)) {
        //Using strcmp because left subtrees are lower in ASCII
        //repeat until the oldspeak matches - that will be the node
        while ((curr != NULL) && (strcmp(curr->oldspeak, oldspeak) != 0)) {
            branches++;
            if (strcmp(curr->oldspeak, oldspeak) > 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
    }
    return curr;
}

//Code inspired by CSE 13s discord and Eric's section on November 24th
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if ((root != NULL) && (oldspeak != NULL)) {
        //traverse/find
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            //sets left root to tree starting with root of left and right
            //essentially the same tree taken down a notch
            branches++;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches++;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) == 0) {
            //duplicate found, do nothing
            return root;
        }
    }
    //insert once at proper spot - child spot open
    //base case for recursion
    if ((root == NULL) && (oldspeak != NULL)) {
        //children will be null.
        root = node_create(oldspeak, newspeak);
    }
    //returns root with new node inserted
    return root;
}

//Method inspired by Professor Long's Lecture 18 inorder traversal code and Eugene's section on November 30th
void bst_print(Node *root) {
    //using recursive in order traversal - left, root, right, starting from the very left
    if (root) {
        if (root == NULL) {
            return;
        }
        //travels down left and right recursively, printing each in order
        if (root->left) {
            bst_print(root->left);
        }
        node_print(root);
        if (root->right) {
            bst_print(root->right);
        }
    }
}
