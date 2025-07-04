#ifndef FORM_TEXT_H 
#define FORM_TEXT_H 

#include "form_style.h"
#include "form_state.h"
typedef void *Text;

/*
 * Create a new text form
 * @param id the id of the text
 * @param x the horizontal coordinate 
 * @param y the vertical coordinate 
 * @param text the text of the form
 * @return the new text
*/
Text new_text(int id, double x, double y, char *text, FormStyle style);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param t the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void get_text_bounding_box(Text t, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param t the form to get the position
 * @param x, y the available positions to return
*/
void get_text_positions(Text t, double *x, double *y);

/* 
 * Gets the state of the text
 * @param t the form to get the state
 * @return the state of the text
*/
FormState get_text_state(Text t);

/* 
 * Gets the id of the text
 * @param t the form to get the id
 * @return the id of the text
*/
int get_text_id(Text t);

/* 
 * Gets the style of the text
 * @param t the form to get the style
 * @return the style of the text
*/
FormStyle get_text_style(Text t); 

/* 
 * Sets the text position to the given coordinates
 * @param t the form to set the position
 * @param x, y the new coordinates of the text
*/
void text_transp(Text t, double x, double y);

/* 
 * Gets the text string of the form
 * @param t the form to get the text
 * @return the text string of the form
*/
char* get_text_string(Text t);

/* 
 * Gets the distance for displacement calculation
 * @param t the form to calculate distance
 * @return the distance value (12 * character count)
 */
double get_text_distance_disp(Text t);

/* 
 * Free memory of the given form
 */
void free_text(Text t); 

#endif