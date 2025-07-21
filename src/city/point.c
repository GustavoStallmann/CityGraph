#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "point.h"

typedef struct Point_st {
    double x, y; 
} Point_st; 

Point new_point(double x, double y) {
    Point_st *point = (Point_st *) malloc(sizeof(Point_st));
    if (point == NULL) {
        fprintf(stderr, "(point) Error: couldn't create a new point instance \n");
        return NULL; 
    }

    point->x = x; 
    point->y = y; 
    return (Point) point; 
}

void point_get_coordinates(Point point, double *x, double *y) {
    assert(point); 
    assert(x); 
    assert(y); 

    Point_st *p = (Point_st *) point; 
    *x = p->x;
    *y = p->y; 
}

void free_point(Point point) {
    assert(point);

    free(point); 
}