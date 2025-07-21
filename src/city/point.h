#ifndef POINT_H
#define POINT_H

/*
 * @file: point.h 
 * @brief: city point implementation

 * this module provides a structure to define a desired point in a city
 * @author: Gustavo Stallmann
*/

// Point structure
typedef void *Point; 

/*
 * Create a new point instance
 * @param "x" the longitudinal coordinate
 * @param "y" the latidudinal coordinate
 * @return "Point" new point instance
*/
Point new_point(double x, double y); 

/*
 * Retrieves coordinates of a point
 * @param "point" the point to get the coordinates
 * @parma "x" pointer of x result
 * @parma "y" pointer of y result 
*/
void point_get_coordinates(Point point, double *x, double *y);

/* 
 * Free up the given point 
 * @param "point" the point to be freed
*/
void free_point(Point point);

#endif