#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "street.h"


typedef struct Street_st {
    char *name; 
} Street_st; 

static char* alloc_str(char *str) {
    if (str == NULL) return NULL; 
    
    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size); //alloc a new str 
    if (new_str == NULL) {
        fprintf(stderr, "(street) Error: couldn't alloc street name (%s)", str);
        return NULL; 
    }; 
    strcpy(new_str, str);
    return new_str; 
}

Street new_street(char *name) {
    Street_st *street = (Street_st *) malloc(sizeof(Street_st));
    if (street == NULL) {
        fprintf(stderr, "(street) Error: couldn't alloc new street instance (%s)", name); 
        return NULL; 
    }

    street->name = alloc_str(name); 

    return (Street) name; 
}