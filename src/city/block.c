#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

typedef struct Block_st {
    char *name; 
    Form block_form; 
} Block_st; 

static char* alloc_str(char *str) {
    int str_len = strlen(str); 

    char *new_str = (char *) malloc(str_len * sizeof(char)); 
    if (new_str == NULL) return NULL; 

    strcpy(new_str, str);
    return new_str;
}

Block block_new(char *name, Form block_form) {
    assert(name);
    assert(block_form); 
    char *alloc_name = alloc_str(name); 
    if (alloc_name == NULL) return NULL; 

    Block_st *block = (Block_st *) malloc(sizeof(Block_st)); 
    if (block == NULL) return NULL; 

    block->name = alloc_name;
    block->block_form = block_form; 

    return (Block) block;
}

void block_set_name(Block b, char *name) {
    assert(b);
    assert(name); 

    Block_st *block = (Block_st *) b;
    if (block->name != NULL) {
        free(block->name); 
    }
    char *new_name = alloc_str(name);
    if (new_name == NULL) return; 

    block->name = new_name; 
}