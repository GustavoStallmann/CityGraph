#ifndef FORM_LINE_H 
#define FORM_LINE_H
#include "form_style.h"
#include "form_state.h"

typedef void *Line;

/*
 * Create a new line form
 * @param x the first point horizontal coordinate
 * @param y the first point vertical coordinate 
 * @param x2 the second point horizontal coordinate
 * @param y2 the second point vertical coordinate
 * @return the new line
*/
Line new_line(double x, double y, double x2, double y2, FormStyle style);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param l the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
*/
void get_line_bounding_box(Line l, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param l the form to get the position
 * @param x, y, x2, y2 the available positions 
*/
void get_line_positions(Line l, double *x, double *y, double *x2, double *y2);

/* 
 * Sets the line position to the given coordinates
 * @param l the form to set the position
 * @param x, y the new coordinates of the line
*/
void line_transp(Line l, double x, double y); 

/* 
 * Gets the state of the line
 * @param l the form to get the state
 * @return the state of the line
*/
FormState get_line_state(Line l);

/* 
 * Gets the style of the line
 * @param l the form to get the style
 * @return the style of the line
*/
FormStyle get_line_style(Line l); 

/* 
 * Gets the distance for displacement calculation
 * @param l the form to calculate distance
 * @return the distance value (10 * line length)
 */
double get_line_distance_disp(Line l);

/* 
 * Free memory of the given form
 */
void free_line(Line l);
 
#endif