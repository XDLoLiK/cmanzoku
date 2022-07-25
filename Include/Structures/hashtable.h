#ifndef Man_HASH_TABLE_H
#define Man_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define HT_NOT_FOUND NULL

enum HashTable_State {
    HT_STATE_Empty   = 0,
    HT_STATE_Deleted = 1,
    HT_STATE_Busy    = 2,
};

struct HashTable_Elem {
    void *key;
    void *value;
};

struct HashTable {
    size_t capacity;
    size_t size;
    size_t keySize;
    size_t valueSize;
    struct HashTable_Elem *data;
    enum HashTable_State *state;
};

// CD
struct HashTable *HashTable_New(size_t capacity, size_t keySize, size_t valueSize);
struct HashTable *HashTable_Delete(struct HashTable *ht);

// RU
size_t HashTable_Size(struct HashTable const *ht);
void *HashTable_Find(struct HashTable *ht, void *key);
int HashTable_Erase(struct HashTable *ht, void *key);
int HashTable_Insert(struct HashTable *ht, void *key, void *value);

// HASH
size_t HashTable_MainHash(struct HashTable const *ht, void *key);
size_t HashTable_SecondaryHash(struct HashTable const *ht, void *key);

#endif // Man_HASH_TABLE_H
