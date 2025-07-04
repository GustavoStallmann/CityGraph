#ifndef FORM_STATE_H
#define FORM_STATE_H

#include <stdbool.h>
typedef void *FormState; 

#define STATE_COUNT 1 // amount of the states types
typedef enum {
    TEST, 
} FormStatesTypes; 

/*
 * Creates a new form state
 * @return a new FormState object
 */
FormState new_form_state();

/*
 * Set the status of the given state
 * @param state the state to change the status
 * @param type the type of the state to be changed
 * @param status the new status of the status
*/
void form_state_set(FormState state, FormStatesTypes type, bool status);

/*
 * Get the current status of the given state
 * @param state the state to get the status
 * @param type the type of the state to get the status
 * @returns bool the current status of the state
*/
bool form_state_get(FormState state, FormStatesTypes type);

/*
 * Frees the memory of the form state
 * @param state the form state to free
 */
void free_form_state(FormState state);

#endif