#include "Structures/hashtable.h"

struct HashTable *HashTable_New(size_t capacity)
{
    struct HashTable *ht = calloc(1, sizeof (struct HashTable));
    if (ht == NULL) {
        return ht;
    }

    ht->data  = calloc(capacity, sizeof (struct HashTableElem));
    ht->state = calloc(capacity, sizeof (enum   HashTableState));
    if (ht->data == NULL || ht->state == NULL) {
        free(ht->data);
        free(ht->state);
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    return ht;
}

int HashTable_Insert(struct HashTable *ht, char *key, char *value)
{
    if ((ht == NULL) || (ht->size + ht->size >= ht->capacity)) {
         return 1;
    }

    size_t elem_hash = HashTable_MainHash(key) % ht->capacity;
    size_t inc = HashTable_SecondaryHash(key);

    for (size_t tries = 0; tries < ht->capacity; elem_hash = (elem_hash + inc) % ht->capacity, tries++) {
        switch (ht->state[elem_hash]) {
            case HT_STATE_Deleted: {
                [[fallthrough]];
            }
            case HT_STATE_Empty: {
                ht->data[elem_hash].key   = calloc(strlen(key)   + 1, sizeof (char));
                ht->data[elem_hash].value = calloc(strlen(value) + 1, sizeof (char));
                strcpy(ht->data[elem_hash].value, value);
                strcpy(ht->data[elem_hash].key, key);
                ht->state[elem_hash] = HT_STATE_Busy;
                ht->size++;
                return 0;
            }
            case HT_STATE_Busy: {
                if (strcmp(key, ht->data[elem_hash].key) != 0) {
                    continue;
                }
                free(ht->data[elem_hash].value);
                ht->data[elem_hash].value = calloc(strlen(value) + 1, sizeof (char));
                strcpy(ht->data[elem_hash].value, value);
                return 0;
            }
            default: {
                return 1;
            }
        }
    }

    return 1;
}

size_t HashTable_Find(struct HashTable *ht, char *key)
{
    if (ht == NULL) {
         return HT_NOT_FOUND;
    }

    size_t elem_hash = HashTable_MainHash(key) % ht->capacity;
    size_t inc = HashTable_SecondaryHash(key);

    for (size_t tries = 0; tries < ht->capacity; elem_hash = (elem_hash + inc) % ht->capacity, tries++) {
        switch (ht->state[elem_hash]) {
            case HT_STATE_Deleted: {
                continue;
            }
            case HT_STATE_Empty: {
                return HT_NOT_FOUND;
            }
            case HT_STATE_Busy: {
                if (strcmp(key, ht->data[elem_hash].key) != 0) {
                    continue;
                }
                return elem_hash;
            }
            default: {
                return HT_NOT_FOUND;
            }
        }
    }

    return HT_NOT_FOUND;
}

int HashTable_Erase(struct HashTable *ht, char *key)
{
    if (ht == NULL) {
        return 1;
    }

    size_t elem_hash = HashTable_MainHash(key) % ht->capacity;
    size_t inc = HashTable_SecondaryHash(key);

    for (size_t tries = 0; tries < ht->capacity; elem_hash = (elem_hash + inc) % ht->capacity, tries++) {
        switch (ht->state[elem_hash]) {
            case HT_STATE_Deleted: {
                continue;
            }
            case HT_STATE_Empty: {
                return 1;
            }
            case HT_STATE_Busy: {
                if (strcmp(key, ht->data[elem_hash].key) != 0) {
                    continue;
                }
                free(ht->data[elem_hash].key);
                free(ht->data[elem_hash].value);
                ht->state[elem_hash] = HT_STATE_Deleted;
                ht->size--;
                return 0;
            }
            default: {
                return 1;
            }
        }
    }

    return 1;
}

size_t HashTable_Size(struct HashTable const *ht)
{
    if (ht == NULL) {
        return 0;
    }
    return ht->size;
}

struct HashTable *HashTable_Delete(struct HashTable *ht)
{
    if (ht == NULL) {
        return ht;
    }
    
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->state[i] == HT_STATE_Busy) {
            free(ht->data[i].value);
            free(ht->data[i].key);            
        }
    }
    free(ht->data);
    free(ht->state);
    free(ht);

    return NULL;
}

// djb2
size_t HashTable_MainHash(char *str)
{
    size_t hash = 5381;
    int cur_symbol = 0;
    
    while ((cur_symbol = *str++)) {
        hash = ((hash << 5) + hash) + cur_symbol;
    }
    
    return hash;
}

size_t HashTable_SecondaryHash(char *str)
{
    size_t p = 31;
    size_t m = 1e9 + 9;
    size_t hash = 0;
    size_t p_pow = 1;
    
    int cur_symbol = 0;
    while ((cur_symbol = *str++)) {
        hash = (hash + (cur_symbol - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    
    return hash;
}
