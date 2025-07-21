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
Block new_block(char *name, Form block_form); 

/*
 * Define a name of the given block instance
 * @param "block" instance of block
 * @param "name" the name to the block
*/
void block_set_name(Block block, char *name); 

/* 
 * Get the form of the given block
 * @param "Block" the block to get the form
 * @return "Form" the form of the block
*/
Form block_get_form(Block b);

/*
 * Get the form of block name 
 * @param "Block" the block to get the text form
 * @return "Form" the text form of the block
*/
Form block_get_form_text(Block b);


/*
 * Free the given form
 */
void block_free(Block b);