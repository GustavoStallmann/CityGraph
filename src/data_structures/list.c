#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct lst_nd {
    ListValue value; 
    ListNode *next; 
} ListNode_st;

typedef struct st_list {
    ListNode *head; 
    int size; 
} List_st;

static void alloc_error() {
    fprintf(stderr, "(ERROR) list: insufficient memory for list allocation");
    exit(EXIT_FAILURE);     
}
 
List new_list() {
    List_st* new_list = (List_st *) malloc(sizeof(List_st));
    if (new_list == NULL) alloc_error( ); 

    new_list->head = NULL; 
    new_list->size = 0; 

    return (List) new_list; 
}

static ListNode new_node(ListValue value, ListNode nextNode) {
    ListNode_st *new_node = (ListNode_st *) malloc(sizeof(ListNode_st));
    if (new_node == NULL) alloc_error( );

    new_node->next = nextNode; 
    new_node->value = value; 

    return (ListNode) new_node;
}
 
ListNode list_insert(List lst, ListValue value) {
    assert(lst); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return NULL; 

    ListNode node = new_node(value, list->head); 

    list->head = node; 
    list->size++; 

    return (ListNode) node;
}

ListNode list_insert_end(List lst, ListValue value) {
    assert(lst); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return NULL; 

    ListNode new_node_ptr = new_node(value, NULL);
    
    if (list->head == NULL) {
        list->head = new_node_ptr;
    } else {
        ListNode_st *current = (ListNode_st *) list->head;
        while (current->next != NULL) {
            current = (ListNode_st *) current->next;
        }
        current->next = new_node_ptr;
    }
    
    list->size++;
    
    return new_node_ptr;
}

struct list_copy_items_data_st {
    List target_list; 
}; 

static void list_copy_items(void *value, callback_data call_data) {
    ListValue list_item = value; 
    struct list_copy_items_data_st *data = call_data; 

    list_insert(data->target_list, list_item); 
}

void list_copy(List source, List target) {
    assert(source);
    assert(target);
    struct list_copy_items_data_st data = {.target_list = target}; 
    list_foreach(source, &list_copy_items, &data);
}

ListValue list_remove(List lst) {
    assert(lst); 

    List_st* list = (List_st *) lst; 
    
    if (list == NULL || list->head == NULL) {
        return NULL; 
    }
    
    ListNode_st *temp = (ListNode_st *) list->head; 
    ListValue temp_value = temp->value;

    list->head = temp->next; 
    list->size--; 
    free(temp); 

    return temp_value; 
}

ListValue list_remove_value(List lst, void *value, compare compare_fn) {
    assert(lst); 
    assert(compare_fn); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return NULL;
    
    ListNode_st *previous = NULL; 
    ListNode_st *current = (ListNode_st *) list->head;
    while (current != NULL) {
        if (compare_fn(current->value, value)) {
            if (previous == NULL) {
                list->head = current->next; 
            } else {
                previous->next = current->next; 
            }
            
            break; 
        }

        previous = current; 
        current = (ListNode_st *) current->next; 
    }

    if (current != NULL) {
        ListValue temp = current->value;
        free(current);
        list->size--; 
        return temp; 
    }
    
    return NULL; 

}

ListValue list_search(List lst, void *value, compare compare_fn) {
    assert(lst); 
    assert(compare_fn); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return NULL;
    
    ListNode_st *current = (ListNode_st *) list->head;
    while (current != NULL) {
        if (compare_fn(current->value, value)) {
            return current->value; 
        }
        current = (ListNode_st *) current->next; 
    }
    
    return NULL; 
}

bool list_includes(List lst, void *value, compare compare_fn) {
    assert(lst); 
    assert(compare_fn); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return false;
    
    ListNode_st *current = (ListNode_st *) list->head;
    while (current != NULL) {
        if (compare_fn(current->value, value)) {
            return true; 
        }
        current = (ListNode_st *) current->next; 
    }
    
    return false; 
}

void list_foreach(List lst, callback_fn callFn, callback_data callSt) {
    assert(lst); 
    assert(callFn); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return;

    ListNode_st *current = (ListNode_st *) list->head;
    while (current != NULL) {
        callFn(current->value, callSt);

        current = (ListNode_st *) current->next; 
    }
}

void list_free(List lst, free_list_value free_value) {
    assert(lst); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return;

    ListNode_st *current = (ListNode_st *) list->head;
    while (current != NULL) {
        ListNode_st *next = (ListNode_st *) current->next;
        
        if (current->value != NULL && free_value != NULL) {
            free_value(current->value);
        }
        
        free(current);
        current = next; 
    }
    
    free(list);
}

int list_get_size(List lst) {
    assert(lst); 

    List_st *list = (List_st *) lst; 
    if (list == NULL) return -1;

    return list->size; 
}