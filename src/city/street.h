#ifndef STREET_H
#define STREET_H

#include <stdbool.h>
 
/*
 * @file: street.h 
 * @brief: city street implementation

 * this module provides a structure to define a desired street of the city
 * @author: Gustavo Stallmann
*/

typedef void *Street; 

/*
 * Create a new instance of street
 * @param "name" the name of the street
 * @param "mean_vel" mean velocity of the street
 * @param "length" the length of the street
 * @return "Street" the new street instance
*/
Street new_street(char *name, double mean_vel, double length); 

/*
 * Set the status of the given street
 * @param "Street" the street to set the status
 * @param "status" the new status
*/
void street_set_status(Street st, bool status); 

/*
 * Get the name of the given street
 * @param "Street" the street to get the name
 * @return "char*" the name of the street
*/
char* street_get_name(Street st); 

/* 
 * Return specs about the given street
 * @param "Street" the street instance
 * @param "*mean_vel" the mean velocity of the street
 * @param "*length" the length of the street
 * @param "*status" the status of the street
 * @param "*name" the name of the street
*/
void street_get_specs(Street st, double *mean_vel, double *length, bool *status, char *name); 

/*
 * Free up the given street
 * @param "Street" the street to be freed
*/
void free_street(Street st); 


#endif  