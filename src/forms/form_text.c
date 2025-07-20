#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "form_text.h"
#include "form_state.h"
#include "form_style.h"

typedef struct {
    char *str; 
    double x, y;
    FormStyle style;
    FormState state; 
} Text_st; 

static char* alloc_str(char *str) {
    assert(str);
    if (str == NULL) return NULL; 

    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size);
    if (new_str == NULL) {
        fprintf(stderr, "(ERROR) form_text: insufficient memory to alloc text");
        return NULL;
    }; 

    strcpy(new_str, str);

    return new_str; 
}

void text_transp(Text t, double x, double y) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return; 

    text->x = x; 
    text->y = y; 
}

Text new_text(double x, double y, char *str, FormStyle style) {
    Text_st *text = (Text_st *) malloc(sizeof(Text_st)); 
    if (text == NULL) {
        fprintf(stderr, "(ERROR) form_text: insufficient memory to alloc text form");
        return NULL;
    }

    text->x = x; 
    text->y = y; 
    text->style = style;
    text->state = new_form_state();
    
    text->str = alloc_str(str); 
    if (text->str == NULL) {
        free_form_style(text->style);
        free_form_state(text->state);
        free(text);
        fprintf(stderr, "(ERROR) form_text: insufficient memory to alloc text string");
        return NULL;
    }
    
    return text; 
}

void get_text_bounding_box(Text t, double *x, double *y, double *w, double *h) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return;

    if (x != NULL) *x = text->x; 
    if (y != NULL) *y = text->y;
    if (w != NULL) *w = 0;
    if (h != NULL) *h = 0; 
}

FormState get_text_state(Text t) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return NULL; 

    return text->state; 
}

FormStyle get_text_style(Text t) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return NULL; 

    return text->style; 
}

char* get_text_string(Text t) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL || text->str == NULL) return ""; 

    return text->str; 
}

void get_text_positions(Text t, double *x, double *y) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return;

    if (x != NULL)
        *x = text->x; 
    if (y != NULL)
        *y = text->y;
}

double get_text_distance_disp(Text t) {
    assert(t);

    Text_st *text = (Text_st *) t;
    if (text == NULL || text->str == NULL) return 0.0;

    // Text distance = 12 * number of characters
    return 12.0 * strlen(text->str);
}

void free_text(Text t) {
    assert(t); 

    Text_st *text = (Text_st *) t; 
    if (text == NULL) return; 

    free(text->str);
    free_form_style(text->style);
    free_form_state(text->state);
    free(text);
}