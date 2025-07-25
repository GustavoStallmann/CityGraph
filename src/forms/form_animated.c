#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#include "form_animated.h"
#include "../data_structures/list.h"
#include "form_style.h"

typedef struct {
    double x, y, r; 
    List path_points;
    FormStyle style; 
} AnimatedForm_st; 

AnimatedForm new_animated_form(double x, double y, double r, List path_points, FormStyle style) {
    AnimatedForm_st *animated_form = (AnimatedForm_st *) malloc(sizeof(AnimatedForm_st)); 
    if (animated_form == NULL) {
        fprintf(stderr, "(ERROR) form_animated: insufficient memory to alloc animated form\n");
        return NULL;
    }

    if (x < 0 || y < 0 || r <= 0) {
        fprintf(stderr, "(ERROR) form_animated: invalid coordinates (x: %lf, y: %lf, r: %lf)\n", x, y, r);
        free(animated_form);
        return NULL;
    }

    if (path_points == NULL) {
        fprintf(stderr, "(ERROR) form_animated: path_points cannot be NULL\n");
        free(animated_form);
        return NULL;
    }

    int total_points = list_get_size(path_points);
    if (total_points <= 0) {
        fprintf(stderr, "(ERROR) form_animated: path_points list is empty\n");
        free(animated_form);
        return NULL;
    }

    animated_form->x = x; 
    animated_form->y = y; 
    animated_form->r = r;
    animated_form->path_points = path_points;
    animated_form->style = style; 

    return animated_form; 
}

List get_animated_form_path_points(AnimatedForm af) {
    assert(af);

    AnimatedForm_st *animated_form = (AnimatedForm_st *) af;
    if (animated_form == NULL) return NULL;

    return animated_form->path_points;
}

void get_animated_form_positions(AnimatedForm af, double *x, double *y, double *r) {
    assert(af);
    assert(x);
    assert(y);

    AnimatedForm_st *animated_form = (AnimatedForm_st *) af;
    if (animated_form == NULL) return;

    *x = animated_form->x;
    *y = animated_form->y;
    if (r != NULL) {
        *r = animated_form->r;
    }
}

FormStyle get_animated_form_style(AnimatedForm af) {
    assert(af); 

    AnimatedForm_st *animated_form = (AnimatedForm_st *) af; 
    if (animated_form == NULL) return NULL; 

    return animated_form->style; 
}

void free_animated_form(AnimatedForm af) {
    assert(af); 

    AnimatedForm_st *animated_form = (AnimatedForm_st *) af; 
    if (animated_form == NULL) return; 

    free_form_style(animated_form->style);
    free(animated_form);
}
