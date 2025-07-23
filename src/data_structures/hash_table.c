#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

typedef struct hNode {
    HashKey key; 
    HashValue value; 
    struct hNode *next; 
} HashNode_st;

typedef struct {
    HashNode_st **table;
    int size; 
} Hash_st;

static HashKey alloc_key(HashKey key) {
    int key_size = strlen(key); 
    char *key_alloc = (char *) malloc(key_size + 1);  
    if (key_alloc == NULL) {
        fprintf(stderr, "[hash_table] Error: insufficient memory to alloc\n"); 
        exit(1); 
    }

    strcpy(key_alloc, key); 
    return key_alloc; 
}

static unsigned hash(Hash_st *table, HashKey key) {
    unsigned hashval; 
    for (hashval = 0; *key != '\0'; key++)
        hashval *= *key + 31 * hashval;

    return hashval % hash_get_size(table);
}

static HashNode_st* new_node(HashKey key, HashValue value, HashNode_st *next) {
    HashNode_st *new_node = (HashNode_st *) malloc(sizeof(HashNode_st));
    if (new_node == NULL) return NULL;

    new_node->key = alloc_key(key); 
    new_node->value = value;
    new_node->next = next;
    return new_node;
}

Hash new_hash(int size) {
    if (size < 0) return NULL;

    Hash_st *hash = (Hash_st *) malloc(sizeof(Hash_st));
    if (hash == NULL) return NULL;

    hash->table = (HashNode_st **) calloc(size, sizeof(HashNode_st));
    if (hash->table == NULL) {
        free(hash); 
        return NULL;
    }
    hash->size = size;
    return hash;
}

void hash_insert(Hash t, HashKey key, HashValue value) {
    assert(t);
    Hash_st *hash_t = (Hash_st *) t;
    unsigned hash_key = hash(hash_t, key);

    HashNode_st *temp = hash_t->table[hash_key]; 

    hash_t->table[hash_key] = new_node(key, value, temp);
}

HashValue hash_get(Hash t, HashKey key) {
    assert(t);
    Hash_st *hash_t = (Hash_st *) t;
    unsigned hash_key = hash(hash_t, key);

    if (hash_t->table == NULL) 
        return NULL; 

    HashNode_st *current_node = hash_t->table[hash_key];
    while (current_node != NULL) {
        if (!strcmp(current_node->key, key)) break; 
        current_node = current_node->next; 
    }

    return current_node != NULL? current_node->value: NULL; 
}

void hash_set(Hash t, HashKey key, HashValue value) {
    assert(t);
    Hash_st *hash_t = (Hash_st *) t;
    unsigned hash_key = hash(hash_t, key);

    HashNode_st *current_node = hash_t->table[hash_key]; 
    while (current_node != NULL) {
        if (!strcmp(current_node->key, key)) break; 
        current_node = current_node->next; 
    }
    
    if (current_node != NULL) {
        current_node->value = value; 
    } else {
        hash_t->table[hash_key] = new_node(key, value, hash_t->table[hash_key]);
    }
}

HashValue hash_remove(Hash t, HashKey key) {
    assert(t);
    Hash_st *hash_t = (Hash_st *) t;
    unsigned hash_key = hash(hash_t, key);

    if (hash_t->table[hash_key] == NULL) 
        return NULL; 

    HashNode_st *previous_node = NULL; 
    HashNode_st *current_node = hash_t->table[hash_key];
    while (current_node != NULL) {
        if (!strcmp(current_node->key, key)) break; 

        previous_node = current_node; 
        current_node = current_node->next; 
    }

    if (current_node == NULL) 
        return NULL; 

    if (previous_node == NULL) {
        hash_t->table[hash_key] = current_node->next; 
    } else {
        previous_node->next = current_node->next; 
    }

    HashValue temp_value = current_node->value; 
    free(current_node->key);
    free(current_node); 
    return temp_value;
}

int hash_get_size(Hash t) {
    assert(t);
    Hash_st *table = (Hash_st *) t; 
    return table->size;
}