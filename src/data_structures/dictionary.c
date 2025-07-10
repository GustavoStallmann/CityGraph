#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "dictionary.h"

typedef struct {
    DictValue *values; 
    int size; 
} Dict_st;

Dict new_dict(int size) {
    assert(size && size > 0); 

    Dict_st *dict = (Dict_st *) malloc(sizeof(Dict_st));
    if (dict == NULL) {
        fprintf(stderr, "[dictionary] Error: insufficient memory to alloc a instance of dict");
        return NULL; 
    }
    
    dict->values = (DictValue *) calloc(size, sizeof(DictValue));
    if (dict->values == NULL) {
        fprintf(stderr, "[dictionary] Error: insufficient memory to alloc a instance of dict");
        free(dict);
        return NULL; 
    }
    
    dict->size = size; 
    return (Dict) dict; 
}

int dict_get_size(Dict dict) {
    Dict_st *d = (Dict_st *) dict;
    return d->size;
}

void dict_put(Dict dict, DictKey key, DictValue value) {
    assert(dict);
    if (key > dict_get_size(dict) || key < 0) return;

    Dict_st *d = (Dict_st *) dict;
    d->values[key] = value; 
}

void dict_remove(Dict dict, DictKey key) {
    assert(dict);
    if (key > dict_get_size(dict) || key < 0) return;
    
    Dict_st *d = (Dict_st *) dict;
    d->values[key] = NULL;
}

DictValue dict_get(Dict dict, DictKey key) {
    assert(dict); 
    if (key > dict_get_size(dict) || key < 0) return NULL;
    
    Dict_st *d = (Dict_st *)dict;
    return d->values[key];
}

bool dict_is_empty(Dict dict, DictKey key) {
    assert(dict); 
    if (key > dict_get_size(dict) || key < 0) return false;

    Dict_st *d = (Dict_st *)dict;
    return d->values[key] == NULL;
}

void dict_free(Dict dict) {
    Dict_st *d = (Dict_st *) dict; 

    free(d->values);
    free(d);
}
