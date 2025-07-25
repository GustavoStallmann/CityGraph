#ifndef DICTIONARY_H
#define DICTIONARY_H

/*
 * @file dictionary.h
 * @brief Generic dictionary implementation

 * Dictionary is a data structure that maps key to values, providing efficient operations
 * @author: Gustavo Stallmann
*/

typedef void *Dict; 
typedef int DictKey; 
typedef void *DictValue; 

/*
 * Create a new dict instance
 * @param "size" the size of the dictionary
 * @return "Dict" the new instance of dict
*/
Dict new_dict(int size); 

/*
 * Put a new key/value in the dict (if key value's not empty it gets replaced)
 * @param "dict" dict instance to receive the new value
 * @param "key" the key of the value
 * @param "value" the new value to be mapped
*/
void dict_put(Dict dict, DictKey key, DictValue value); 

/*
 * Remove a value from a dict based on its key
 * @param "dict" dict instance to remove the value
 * @param "key" the key to clear the value
*/
void dict_remove(Dict dict, DictKey key);

/*
 * Get a value from a key
 * @param "dict" dict instance to get the value
 * @param "key" the key to get the value from
 * @return "DictValue" the value associated with the key
*/
DictValue dict_get(Dict dict, DictKey key);

/*
 * Checks if a key is empty or not 
 * @param "dict" dict instance
 * @param "key" the key to be checked
 * @return true if its empty, false otherwise
*/
bool dict_is_empty(Dict, DictKey key);

/*
 * Get the size of the given dict
 * @param "dict" instance to get the size
 * @return the size of the dict
*/
int dict_get_size(Dict dict); 

/*
 * Free the given dict
 * @param "dict" the instance of dict to be freed
*/
void dict_free(Dict dict); 

#endif