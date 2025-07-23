#ifndef FORM_H
#define FORM_H

#include "form_style.h"
#include "form_state.h"
#include "../data_structures/list.h"

typedef enum {
    CIRCLE, 
    RECT, 
    TEXT, 
    LINE,
    ANIMATED
} FormType;

// typedef int FormType; 
typedef void* Form;

/*
 * Create a new form 
 * @param type the type of the new form
 * @param id the identifier of the form
 * @param x the horizontal axis of the form
 * @param y the vertical axis of the form
 * @param wr the width/radius/x2 of the form (depending on the type)
 * @param h the height/y2 of the form (depending on the type)
 * @param text the text of the form (available for text type)
 * @param style the style of the form
 * @return the new form created
*/
Form new_form(FormType type, int id, double x, double y, double wr, double h, char *text, FormStyle style);

/*
 * Create a new animated form that follows a path
 * @param id the identifier of the form
 * @param x the starting horizontal coordinate
 * @param y the starting vertical coordinate
 * @param r the radius of the animated form
 * @param path_points the list of points to follow (from A* algorithm)
 * @return the new animated form created
*/
Form new_animated_form_wrapper(int id, double x, double y, double r, List path_points);

/*
 * Gets the path points list for an animated form (for SVG export)
 * @param form the animated form
 * @return the list of path points, or NULL if not an animated form
 */
List form_get_path_points(Form form);

/* 
 * Sets the minimum bounding box of the form in the given variables
 * @param form the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
 */
void form_get_bounding_box(Form form, double *x, double *y, double *w, double *h);

/*
 * Gets the style of the form
 * @param form the form to get the style
 * @return the style of the form
 */
FormStyle form_get_style(Form form);

/*
 * Gets the coordinates of the form
 * @param form the form to get the coordinates
 * @param x, y the results of the coordinates
 */
void form_get_coordinates(Form form, double *x, double *y);

/*
 * Gets the dimensions of the form
 * @param tp the type of the form
 * @param form the form to get the dimensions
 * @param w, h the results of the width and height
 */
void form_get_dimensions(Form form, double *w, double *h);

/*
 * Gets the state of the form
 * @param form the form to get the state
 * @return the state of the form
 */
FormState form_get_state(Form form);

/*
 * Gets the name of the form type
 * @param tp the type of the form
 * @return the name of the form type as a string
 */
char* form_get_name(Form form);

/* 
 * Translates the form to the given coordinates
 * @param form the form to translate
 * @param x, y the new coordinates of the form
*/
void form_transp(Form form, double x, double y); 

/*
 * Gets the id of the form
 * @param form the form to get the id
 * @return the id of the form 
*/
int form_get_id(Form form);

/*
 * Gets the type of the given form
 * @param form the form to get the type
 * @return FormType the type of the form
*/
FormType form_get_type(Form f); 

/*
 * Gets the text of a text form
 * @param form the text form
 * @return "char*" the text assigned to the form
*/
char* form_get_text(Form f);

/*
 * Frees the memory of the form
 * @param form the form to free
 */
void form_free(Form form);

#endif