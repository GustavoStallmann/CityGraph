#ifndef FORM_H
#define FORM_H

#include "form_style.h"
#include "form_state.h"

typedef enum {
    CIRCLE, 
    RECT, 
    TEXT, 
    LINE
} FormType;

// typedef int FormType; 
typedef void* Form;

/* 
 * Sets the minimum bounding box of the form in the given variables
 * @param form the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
 */
void get_form_bounding_box(Form form, double *x, double *y, double *w, double *h);

/*
 * Gets the style of the form
 * @param form the form to get the style
 * @return the style of the form
 */
FormStyle get_form_style(Form form);

/*
 * Gets the coordinates of the form
 * @param form the form to get the coordinates
 * @param x, y the results of the coordinates
 */
void get_form_coordinates(Form form, double *x, double *y);

/*
 * Gets the dimensions of the form
 * @param tp the type of the form
 * @param form the form to get the dimensions
 * @param w, h the results of the width and height
 */
void get_form_dimensions(Form form, double *w, double *h);

/*
 * Gets the state of the form
 * @param form the form to get the state
 * @return the state of the form
 */
FormState get_form_state(Form form);

/*
 * Gets the name of the form type
 * @param tp the type of the form
 * @return the name of the form type as a string
 */
char* get_form_name(Form form);

/* 
 * Translates the form to the given coordinates
 * @param form the form to translate
 * @param x, y the new coordinates of the form
*/
void transp_form(Form form, double x, double y); 

/*
 * Gets the id of the form
 * @param form the form to get the id
 * @return the id of the form 
*/
int get_form_id(Form form);

/*
 * Frees the memory of the form
 * @param form the form to free
 */
void free_form(Form form);

#endif