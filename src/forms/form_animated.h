#ifndef FORM_ANIMATED_H
#define FORM_ANIMATED_H

#include "../data_structures/list.h"
#include "../city/point.h"

typedef void *AnimatedForm;

/*
 * Create a new animated form
 * @param x the starting horizontal coordinate 
 * @param y the starting vertical coordinate 
 * @param r the radius of the animated form
 * @param path_points the list of points to follow (from A* algorithm)
 * @return the new animated form
 */
AnimatedForm new_animated_form(double x, double y, double r, List path_points);

/*
 * Gets the path points list for SVG export
 * @param af the animated form
 * @return the list of path points
 */
List get_animated_form_path_points(AnimatedForm af);

/* 
 * Free memory of the given form
 * @param af the form to free
 */
void free_animated_form(AnimatedForm af);

#endif
