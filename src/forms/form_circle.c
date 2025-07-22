#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif
 
#include "form_circle.h"
#include "form_state.h"
#include "form_style.h"

typedef struct {
    double x, y, r; 
    FormStyle style;
    FormState state;
} Circle_st; 

Circle new_circle(double x, double y, double r, FormStyle style) {
    Circle_st *circle = (Circle_st *) malloc(sizeof(Circle_st)); 
    if (circle == NULL) {
        fprintf(stderr, "(ERROR) form_circle: insufficient memory to alloc circle\n");
        return NULL;
    }

    if (x < 0 || y < 0 || r <= 0) {
        fprintf(stderr, "(ERROR) form_circle: invalid coordinates (x: %lf, y: %lf, r: %lf)\n", x, y, r);
        free_form_style(style);
        free(circle);
        return NULL;
    }

    circle->x = x; 
    circle->y = y; 
    circle->r = r;
    circle->style = style; 
    circle->state = new_form_state();

    return circle; 
}

void get_circle_bounding_box(Circle c, double *x, double *y, double *w, double *h) {
    assert(c);
    
    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return; 

    if (x != NULL)
        *x = circle->x - circle->r; 
    if (y != NULL)
        *y = circle->y - circle->r; 
    if (w != NULL)
        *w = circle->r * 2; 
    if (h != NULL)
        *h = circle->r * 2; 
}

void get_circle_positions(Circle c, double *x, double *y, double *r) {
    assert(c);
    
    Circle_st *circle = (Circle_st *) c;
    if (circle == NULL) return; 

    if (x != NULL)
        *x = circle->x;
    if (y != NULL)
        *y = circle->y; 
    if (r != NULL)
        *r = circle->r; 
}

void circle_transp(Circle c, double x, double y) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return; 

    circle->x = x; 
    circle->y = y; 
}

FormStyle get_circle_style(Circle c) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return NULL; 

    return circle->style; 
}

FormState get_circle_state(Circle c) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return NULL; 

    return circle->state; 
}

double get_circle_distance_disp(Circle c) {
    assert(c);

    Circle_st *circle = (Circle_st *) c;
    if (circle == NULL) return 0.0;

    return PI * circle->r * circle->r;
}

void free_circle(Circle c) {
    assert(c); 

    Circle_st *circle = (Circle_st *) c; 
    if (circle == NULL) return; 

    free_form_style(circle->style);
    free_form_state(circle->state);
    free(circle);
}