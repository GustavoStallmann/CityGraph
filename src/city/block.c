#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

typedef struct Block_st {
    char *name; 
    Form block_form; 
    Form block_text;
} Block_st; 

static char* alloc_str(char *str) {
    int str_len = strlen(str); 

    char *new_str = (char *) malloc((str_len + 1) * sizeof(char)); 
    if (new_str == NULL) return NULL; 

    strcpy(new_str, str);
    return new_str;
}

Block new_block(char *name, Form block_form) {
    assert(name);
    assert(block_form); 

    char *alloc_name = alloc_str(name); 
    if (alloc_name == NULL) return NULL; 

    Block_st *block = (Block_st *) malloc(sizeof(Block_st)); 
    if (block == NULL) return NULL; 

    block->name = alloc_name;
    block->block_form = block_form; 

    double x, y; 
    form_get_coordinates(block_form, &x, &y);
    block->block_text = new_form(TEXT, -1, x, y, 0, 0, name, NULL); 
    if (block->block_text == NULL) {
        fprintf(stderr, "(block) Error: couldn't alloc block text\n");
        return NULL; 
    }

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

Form block_get_form(Block b) {
    assert(b); 
    
    Block_st *block = (Block_st *) b;
    if (block->block_form == NULL) return NULL; 

    return block->block_form; 
}

Form block_get_form_text(Block b) {
    assert(b); 
    
    Block_st *block = (Block_st *) b;
    if (block->block_text == NULL) return NULL; 

    return block->block_text; 
}

void block_get_adress_coordinate(Block b, char face, int number, int *x, int *y) {
    Block_st *block = (Block_st *) b;
    double bx, by; 
    form_get_coordinates(block->block_form, &bx, &by);

    if (face == 'N' || face == 'S') {
        *x = bx + number; 
        *y = by; 
    } else if (face == 'L' || face == 'O') {
        *x = bx; 
        *y = by + number; 
    } else {
        fprintf(stderr, "(block) Error: got invalid face type (%c)", face); 
    }
}

void block_free(Block b) {
    assert(b);

    Block_st *block = (Block_st *) b;
    if (block->name != NULL) {
        free(block->name);
    }

    free(b); 
}