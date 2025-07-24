#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef void *PriorityQueue; 
typedef void *QueueValue; 

typedef void (*free_queue_value)(QueueValue value); 

/* 
 * Create a new queue instance
 * @param "capacity" initial capacity of the queue > 0
 * @return "PriorityQueue" queue instance
 */
PriorityQueue new_priority_queue(int capacity);

/* 
 * Insert a new value into the queue
 * @param "queue" the queue instance to insert the value
 * @param "value" the value to be inserted
 * @param "priority" the double priority of the value (lower values = higher priority)
 * @return "bool" true if successful, false otherwise
 */
bool pqueue_enqueue(PriorityQueue queue, QueueValue value, double priority);

/* 
 * Remove and return the lowest priority value from the queue
 * @param "queue" the queue instance
 * @return "QueueValue" the removed value, NULL if queue is empty
 */
QueueValue pqueue_dequeue(PriorityQueue queue);

/* 
 * Get the lowest priority value without removing it
 * @param "queue" the queue instance
 * @return "QueueValue" the lowest priority value, NULL if queue is empty
 */
QueueValue pqueue_peek(PriorityQueue queue);

/* 
 * Get the priority of the lowest priority item without removing it
 * @param "queue" the queue instance
 * @return "double" the priority of the lowest priority item, -1.0 if queue is empty
 */
double pqueue_peek_priority(PriorityQueue queue);

/* 
 * Check if the queue is empty
 * @param "queue" queue instance to verify
 * @return "bool" true if empty, false otherwise
 */
bool pqueue_is_empty(PriorityQueue queue);

/* 
 * Get the number of elements in the queue
 * @param "queue" queue instance
 * @return "int" the number of elements
 */
int pqueue_size(PriorityQueue queue);

/* 
 * Free the queue and all its elements
 * @param "queue" the queue instance
 * @param "free_fn" function to free values
 */
void pqueue_free(PriorityQueue queue, free_queue_value free_fn);

/* 
 * Clear all elements from the priority queue
 * @param "queue" the queue instance 
 * @param "free_fn" function to free values
 */
void pqueue_clear(PriorityQueue queue, free_queue_value free_fn);

#endif
