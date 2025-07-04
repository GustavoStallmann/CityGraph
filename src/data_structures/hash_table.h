#ifndef HASH_H
#define HASH_H

typedef void* Hash; 
typedef char* HashKey;
typedef void* HashValue; 

/*
 * Create a new hash data structure
 * @param size the size of the hash map
 * @return Hash the new data structure
*/
Hash new_hash(int size); 

/*
 * Insert a new value on the given hash table
 * @param table the table to insert the value
 * @param key the key of the value
 * @param value the value to be inserted
*/
void hash_insert(Hash table, HashKey key, HashValue value);

/* 
 * Get a value with a key on the given table
 * @param table the table to get the value
 * @param key the key to get the value
 * @return HashValue the value of the given key
*/
HashValue hash_get(Hash table, HashKey key);

/* 
 * Set a value with a key on the given table
 * @param table the table to set the value
 * @param key the key to set the value 
 * @param value the value to be setted 
*/
void hash_set(Hash table, HashKey key, HashValue value);

/* 
 * Remove a value from the given hash table
 * @param table the table to remove the value
 * @param key the of the value to be removed
 * @return HashValue the value that was stored
*/
HashValue hash_remove(Hash table, HashKey key);

/*
 * Free the given hash table instance
 * @param table the table to be freed
*/
void hash_free(Hash table); 

/*
 * Get the size of the given hash
 * @param table the table to get the value
*/
int hash_get_size(Hash table);


#endif