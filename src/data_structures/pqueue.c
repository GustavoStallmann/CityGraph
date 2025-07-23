#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pqueue.h"

typedef struct pq_item {
    QueueValue value;
    double priority;
} PQItem;

typedef struct pq_st {
    PQItem *heap;
    int size;
    int capacity;
} PriorityQueue_st;

static void alloc_error() {
    fprintf(stderr, "(ERROR) priority_queue: insufficient memory for queue allocation");
    exit(EXIT_FAILURE);     
}

static int parent_index(int index) {
    return (index - 1) / 2;
}

static int left_child_index(int index) {
    return 2 * index + 1;
}

static int right_child_index(int index) {
    return 2 * index + 2;
}

static void swap(PQItem *heap, int i, int j) {
    PQItem temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

static void heapify_up(PriorityQueue_st *pq, int index) {
    assert(pq);
    
    if (index == 0) return;
    
    int parent = parent_index(index);
    if (pq->heap[index].priority > pq->heap[parent].priority) {
        swap(pq->heap, index, parent);
        heapify_up(pq, parent);
    }
}

static void heapify_down(PriorityQueue_st *pq, int index) {
    assert(pq);
    
    int left = left_child_index(index);
    int right = right_child_index(index);
    int largest = index;
    
    if (left < pq->size && pq->heap[left].priority > pq->heap[largest].priority) {
        largest = left;
    }
    
    if (right < pq->size && pq->heap[right].priority > pq->heap[largest].priority) {
        largest = right;
    }
    
    if (largest != index) {
        swap(pq->heap, index, largest);
        heapify_down(pq, largest);
    }
}

PriorityQueue new_priority_queue(int capacity) {
    assert(capacity > 0);
    
    PriorityQueue_st *new_pq = (PriorityQueue_st *) malloc(sizeof(PriorityQueue_st));
    if (new_pq == NULL) alloc_error();
    
    new_pq->heap = (PQItem *) malloc(capacity * sizeof(PQItem));
    if (new_pq->heap == NULL) alloc_error();
    
    new_pq->size = 0;
    new_pq->capacity = capacity;
    
    return (PriorityQueue) new_pq;
}

bool pqueue_enqueue(PriorityQueue queue, QueueValue value, double priority) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    if (pq->size >= pq->capacity) {
        return false;
    }
    
    pq->heap[pq->size].value = value;
    pq->heap[pq->size].priority = priority;
    heapify_up(pq, pq->size);
    pq->size++;
    
    return true;
}

QueueValue pqueue_dequeue(PriorityQueue queue) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    if (pq->size == 0) return NULL;
    
    QueueValue max_value = pq->heap[0].value;
    
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    
    if (pq->size > 0) {
        heapify_down(pq, 0);
    }
    
    return max_value;
}

QueueValue pqueue_peek(PriorityQueue queue) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    if (pq->size == 0) return NULL;
    
    return pq->heap[0].value;
}

double pqueue_peek_priority(PriorityQueue queue) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    if (pq->size == 0) return -1.0;
    
    return pq->heap[0].priority;
}

bool pqueue_is_empty(PriorityQueue queue) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    return pq->size == 0;
}

int pqueue_size(PriorityQueue queue) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    return pq->size;
}

void pqueue_clear(PriorityQueue queue, free_queue_value free_fn) {
    assert(queue);
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;

    if (free_fn != NULL) {
        for (int i = 0; i < pq->size; i++) {
            free_fn(pq->heap[i].value);
        }
    }
    
    pq->size = 0;
}

void pqueue_free(PriorityQueue queue, free_queue_value free_fn) {
    if (queue == NULL) return;
    
    PriorityQueue_st *pq = (PriorityQueue_st *) queue;
    if (free_fn != NULL) {
        pqueue_clear(queue, free_fn);
    }
    
    free(pq->heap);
    free(pq);
}
