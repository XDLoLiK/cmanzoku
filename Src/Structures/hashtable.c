#include "Structures/hashtable.h"

struct HashTable *HashTable_New(size_t capacity, size_t keySize, size_t valueSize)
{
    struct HashTable *ht = calloc(1, sizeof (struct HashTable));
    if (ht == NULL) {
        return ht;
    }

    ht->data  = calloc(capacity, sizeof (struct HashTable_Elem));
    ht->state = calloc(capacity, sizeof (enum   HashTable_State));
    if (ht->data == NULL || ht->state == NULL) {
        return HashTable_Delete(ht);
    }

    ht->capacity  = capacity;
    ht->keySize   = keySize;
    ht->valueSize = valueSize;
    return ht;
}

int HashTable_Insert(struct HashTable *ht, void *key, void *value)
{
    if ((ht == NULL) || (ht->size + ht->size >= ht->capacity)) {
         return 1;
    }

    size_t hash = HashTable_MainHash(ht, key) % ht->capacity;
    size_t inc  = HashTable_SecondaryHash(ht, key);

    for (size_t tries = 0; tries < ht->capacity; hash = (hash + inc) % ht->capacity, tries++) {
        switch (ht->state[hash]) {
            case HT_STATE_Deleted:
                __attribute__((fallthrough));

            case HT_STATE_Empty:
                ht->data[hash].key   = calloc(1, ht->keySize);
                ht->data[hash].value = calloc(1, ht->valueSize);
                if (ht->data[hash].key == NULL || ht->data[hash].value == NULL) {
                    free(key);
                    free(value);
                    return 1;
                }
                memcpy(ht->data[hash].key,   key,   ht->keySize);
                memcpy(ht->data[hash].value, value, ht->valueSize);
                ht->state[hash] = HT_STATE_Busy;
                ht->size++;
                return 0;

            case HT_STATE_Busy:
                if (memcmp(key, ht->data[hash].key, ht->keySize) != 0) {
                    continue;
                }
                memcpy(ht->data[hash].value, value, ht->valueSize);
                return 0;

            default:
                return 1;
        }
    }

    return 1;
}

void *HashTable_Find(struct HashTable *ht, void *key)
{
    if (ht == NULL) {
         return HT_NOT_FOUND;
    }

    size_t hash = HashTable_MainHash(ht, key) % ht->capacity;
    size_t inc  = HashTable_SecondaryHash(ht, key);

    for (size_t tries = 0; tries < ht->capacity; hash = (hash + inc) % ht->capacity, tries++) {
        switch (ht->state[hash]) {
            case HT_STATE_Deleted:
                continue;

            case HT_STATE_Empty:
                return HT_NOT_FOUND;

            case HT_STATE_Busy:
                if (memcmp(key, ht->data[hash].key, ht->keySize) != 0) {
                    continue;
                }
                return ht->data[hash].value;

            default:
                return HT_NOT_FOUND;
        }
    }

    return HT_NOT_FOUND;
}

int HashTable_Erase(struct HashTable *ht, void *key)
{
    if (ht == NULL) {
        return 1;
    }

    size_t hash = HashTable_MainHash(ht, key) % ht->capacity;
    size_t inc  = HashTable_SecondaryHash(ht, key);

    for (size_t tries = 0; tries < ht->capacity; hash = (hash + inc) % ht->capacity, tries++) {
        switch (ht->state[hash]) {
            case HT_STATE_Deleted:
                continue;

            case HT_STATE_Empty:
                return 1;

            case HT_STATE_Busy:
                if (memcmp(key, ht->data[hash].key, ht->keySize) != 0) {
                    continue;
                }
                free(ht->data[hash].key);
                free(ht->data[hash].value);
                ht->state[hash] = HT_STATE_Deleted;
                ht->size--;
                return 0;

            default:
                return 1;
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
    
    if (ht->data != NULL && ht->state != NULL) {
        for (size_t i = 0; i < ht->capacity; ++i) {
            free(ht->data[i].key);
            free(ht->data[i].value);
        }   
    }
    
    free(ht->data);
    free(ht->state);
    free(ht);
    return NULL;
}

// djb2
size_t HashTable_MainHash(struct HashTable const *ht, void *key)
{
    size_t hash = 5381;
    
    for (size_t i = 0; i < ht->keySize; i++) {
        hash = ((hash << 5) + hash) + *((char *)key);
        key = ((char *)key) + 1;
    }
    
    return hash;
}

size_t HashTable_SecondaryHash(struct HashTable const *ht, void *key)
{
    size_t p = 31;
    size_t m = 1e9 + 9;
    size_t hash = 0;
    size_t p_pow = 1;
    
    for (size_t i = 0; i < ht->keySize; i++) {
        hash = (hash + (*((char *)key) - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
        key = ((char *)key) + 1;
    }
    
    return hash;
}
