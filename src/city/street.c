#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "street.h"


typedef struct Street_st {
    char *name; 
    bool status; 
    double mean_vel, length; 
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

Street new_street(char *name, double mean_vel, double length) {
    Street_st *street = (Street_st *) malloc(sizeof(Street_st));
    if (street == NULL) {
        fprintf(stderr, "(street) Error: couldn't alloc new street instance (%s)", name); 
        return NULL; 
    }

    street->name = alloc_str(name); 
    street->mean_vel = mean_vel; 
    street->length = length; 

    return (Street) name; 
}

void street_set_status(Street st, bool status) {
    assert(st);
    Street_st *street = (Street_st *) st; 

    street->status = status;     
}

void street_get_specs(Street st, double *mean_vel, double *length) {
    assert(st);
    Street_st *street = (Street_st *) st; 

    *mean_vel = street->mean_vel;
    *length = street->length;
}

void free_street(Street st) {
    assert(st);
    Street_st *street = (Street_st *) st; 

    free(street->name); 
    free(street);
}