#ifndef Man_HASH_TABLE_H
#define Man_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#define HT_NOT_FOUND SIZE_MAX
#define FUNCTION     -1

enum HashTableState {
    HT_STATE_Empty   = 0,
    HT_STATE_Deleted = 1,
    HT_STATE_Busy    = 2,
};

struct HashTableElem {
    char *identifier;
    char *scope;
    int value;
};

struct HashTable {
    size_t capacity;
    size_t size;
    struct HashTableElem *data;
    enum HashTableState *state;
};

// CD
struct HashTable *HashTable_New(size_t capacity);
struct HashTable *HashTable_Delete(struct HashTable *ht);

// RU
size_t HashTable_Size(struct HashTable const *ht);
size_t HashTable_Find(struct HashTable *ht, char *identifier, char *scope);
int HashTable_Erase(struct HashTable *ht, char *identifier, char *scope);
int HashTable_Insert(struct HashTable *ht, char *identifier, char *scope, int value);

// HASH
size_t HashTable_MainHash(char *str);
size_t HashTable_SecondaryHash(char *str);

#endif // Man_HASH_TABLE_H
