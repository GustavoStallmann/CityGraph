#ifndef FORM_CIRCLE
#define FORM_CIRCLE
#include "form_style.h"
#include "form_state.h"
typedef void *Circle;

/*
 * Create a new circle form
 * @param x the horizontal coordinate 
 * @param y the vertical coordinate 
 * @param r the radius of the circle
 * @return the new circle
 */
Circle new_circle(double x, double y, double r, FormStyle style);

/*
 * Sets the minimum bounding box of the form in the given variables
 * @param c the form to get the bounding box
 * @param x, y, w, h the results of the bounding box coordinates
 */
void get_circle_bounding_box(Circle c, double *x, double *y, double *w, double *h);

/* 
 * Sets the form position to the given variables
 * @param c the form to get the position
 * @param x, y, r the available positions 
 */
void get_circle_positions(Circle c, double *x, double *y, double *r);

/* 
 * Sets the circle position to the given coordinates
 * @param c the form to set the position
 * @param x, y the new coordinates of the circle
 */
void circle_transp(Circle c, double x, double y);

/* 
 * Gets the style of the circle
 * @param c the form to get the style
 * @return the style of the circle
 */
FormStyle get_circle_style(Circle c);

/* 
 * Gets the state of the circle
 * @param c the form to get the state
 * @return the state of the circle
 */
FormState get_circle_state(Circle c);

/* 
 * Gets the distance for displacement calculation
 * @param c the form to calculate distance
 * @return the distance value (area = π * r²)
 */
double get_circle_distance_disp(Circle c);

/* 
 * Free memory of the given form
 * @param c the form to free
 */
void free_circle(Circle c);

#endif