/*
 * @file: block.h
 * @brief: residential block implementation

 * This module provides a block management for a city structure
 * @author: Gustavo Freire
*/
#include "../forms/form.h"
typedef void *Block; 


/*
 * Create a new instance of a block
 * @param "name" the name of the block
 * @param "block_form" the form instance for the block
 * @return "Block" the new instance of Block
*/
Block block_new(char *name, Form block_form); 

/*
 * Define a name of the given block instance
 * @param "block" instance of block
 * @param "name" the name to the block
*/
void block_set_name(Block block, char *name); 