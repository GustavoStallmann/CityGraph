#ifndef PROCESSOR_GEO 
#define PROCESSOR_GEO 
#include "processor_dir.h"
#include "../data_structures/list.h"
#include "../data_structures/hash_table.h"

/* 
 * Process the geo file
 * @param dir the geo file directory
 * @param "hash_table" hash register for the blocks
 * @return the list of processed forms from the geo file
*/
List geo_process(Dir dir, Hash hash_table); 

#endif