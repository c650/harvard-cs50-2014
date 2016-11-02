/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

#define LENGTH 45
#define NODE_ARR_LEN 27

// let's declare trie
typedef struct node {
    bool exists;
    struct node** next;
} node;

struct node** first; //global var for dictionary. 
unsigned int dictSize = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word) {

    if (*word == '\0') {
        return false;
    }

    node** curr_level = first;

    bool yes = true;
    for (int i = 0, n = strlen(word), idx; i < n; i++) {
        
        /* This might be source of segfault*/
        if (curr_level == NULL) {
            yes = false;
            break;
        }

        if (word[i] >= 'a' && word[i] <= 'z') {
            idx = word[i] - 'a';
        } else if (word[i] >= 'A' && word[i] <= 'Z') {
            idx = word[i] - 'A';
        } else if (word[i] == '\'') {
            idx = 26; // last elem in array is for '
        }

        if (curr_level[idx] != NULL) {
            if (i == n-1) {
                yes = curr_level[idx]->exists;
                break;
            } else {
                curr_level = curr_level[idx]->next;
            }
        } else {
            yes = false;
            break;
        }
    }
    return yes;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary) {
    first = malloc(sizeof(node*) * NODE_ARR_LEN);

	FILE* fp;
    fp = fopen(dictionary, "r");
    if (fp == NULL) {
        return false;
    }

    node** curr_level = first;
    node** last;
    int idx, word_len = 0;
    for (char tmp = fgetc(fp); tmp != EOF; tmp = fgetc(fp)) {

        if (isalpha(tmp) || tmp == '\'') {
            if (tmp >= 'a' && tmp <= 'z') {
                idx = tmp - 'a';
            } else if (tmp >= 'A' && tmp <= 'Z') {
                idx = tmp - 'A';
            } else if (tmp == '\'') {
                idx = 26;
            }

            if (curr_level[idx] == NULL) {
                // make a new node
                curr_level[idx] = malloc(sizeof(node));
                curr_level[idx]->exists = false; // avoid false-positives, will become true ~ln91
            }

            if (curr_level[idx]->next == NULL) {
                // prepare array
                curr_level[idx]->next = malloc(sizeof(node*) * NODE_ARR_LEN);
            }

            last = curr_level;
            curr_level = curr_level[idx]->next;

            word_len++;
        } else if (word_len > 0) {

            if (last[idx] == NULL) {
                last[idx] = malloc(sizeof(node));
            }

            last[idx]->exists = true;

            curr_level = last = first;
            dictSize++;
            word_len = 0;
        }
    }

    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void) {
    return dictSize;
}

void deallocate(node** n) {
    if (n == NULL) {
        return;
    }
    for (int i = 0; i < NODE_ARR_LEN; i++) {

        if (n[i] != NULL) {
            if (n[i]->next != NULL) {
                deallocate(n[i]->next);
            }
            free(n[i]);
        }
    }
    free(n);
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void) {
    deallocate(first);
    return true;
}
/*
	SOURCES:
	
	http://cs50.stackexchange.com/questions/9460/pset5-speller-c-check
*/