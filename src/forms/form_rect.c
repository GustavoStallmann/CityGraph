#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "form_rect.h"
#include "form_state.h"
#include "form_style.h"

typedef struct {
    int id; 
    double x, y, w, h; 
    FormStyle style;
    FormState state; 
} Rect_st;

Rect new_rect(int id, double x, double y, double w, double h, FormStyle style) {
    Rect_st *rect = (Rect_st *) malloc(sizeof(Rect_st)); 
    if (rect == NULL) {
        fprintf(stderr, "(ERROR) form_rect: insufficient memory for rectangle form allocation");
        return NULL; 
    }

    rect->id = id; 
    rect->x = x; 
    rect->y = y; 
    rect->w = w; 
    rect->h = h; 
    rect->style = style;
    rect->state = new_form_state();

    return rect; 
}

void rect_transp(Rect r, double x, double y) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    rect->x = x; 
    rect->y = y; 
}

int get_rect_id(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return -1; 

    return rect->id; 
}

void get_rect_bounding_box(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    *x = rect->x; 
    *y = rect->y; 
    *w = rect->w; 
    *h = rect->h; 
}

FormStyle get_rect_style(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return NULL; 

    return rect->style; 
}

FormState get_rect_state(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return NULL; 

    return rect->state; 
}

void get_rect_positions(Rect r, double *x, double *y, double *w, double *h) {
    assert(r); 

    Rect_st* rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    if (x != NULL)
        *x = rect->x; 
    if (y != NULL)
        *y = rect->y; 
    if (w != NULL)
        *w = rect->w;   
    if (h != NULL)
        *h = rect->h; 
}

double get_rect_distance_disp(Rect r) {
    assert(r);

    Rect_st *rect = (Rect_st *) r;
    if (rect == NULL) return 0.0;

    // Rectangle distance = area = width * height
    return rect->w * rect->h;
}

void free_rect(Rect r) {
    assert(r); 

    Rect_st *rect = (Rect_st *) r; 
    if (rect == NULL) return; 

    free_form_style(rect->style);
    free_form_state(rect->state);
    free(rect);
}