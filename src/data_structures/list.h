#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef void *List; 
typedef void *ListNode; 
typedef void *ListValue; 

typedef bool (*compare) (ListValue value, void *target); 
typedef void (*free_list_value)(ListValue value); 
typedef void *callback_data;
typedef void (*callback_fn)(void *value, callback_data call_data);

/* 
 * Create a new linked list instance
 * @return List The new created list
 */
List new_list();

/* 
 * Insert a new value on a given list
 * @param list the list to insert the value
 * @param value the value to be inserted
 * @return list the given list
 */
ListNode list_insert(List list, ListValue value);

/* 
 * Insert a new value at the end of a given list
 * @param list the list to insert the value
 * @param value the value to be inserted
 * @return ListNode the newly created node
 */
ListNode list_insert_end(List list, ListValue value);

/* 
 * Remove the last value of a list
 * The user is responsible to free the ListValue
 * @param list the list to remove the value
 * @return ListValue the removed value
*/
ListValue list_remove(List list);

/* 
 * Search for a value on the given list
 * @param list the list to search the value
 * @param value the value to be searched 
 * @return ListValue if found NULL if not
 */
ListValue list_search(List list, void *value, compare f);

/* 
 * Check if the list includes a specific value
 * @param list the list to check
 * @param value the value to be searched
 * @param compare_fn the comparison function
 * @return bool true if the value is found, false otherwise
 */
bool list_includes(List list, void *value, compare compare_fn);

/* 
 * Execute a function for each item in the list
 * @param list the list to iterate
 * @param foreach the function to be executed
 */
void list_foreach(List list, callback_fn callFn, callback_data callSt); 

/* 
 * Free the memory of the given list
 * @param list the list to get free
 */ 
void list_free(List list, free_list_value func); 

/* 
 * Get the amount of elements on the given list
 * @param list the list to get the size
 * @return int: the amount of elements
*/
int list_get_size(List list); 

#endif