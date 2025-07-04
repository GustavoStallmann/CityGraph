#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "form_state.h"

typedef struct {
    bool *states; 
} FormState_st;

FormState new_form_state() {
    FormState_st *state = (FormState_st *) malloc(sizeof(FormState_st));
    if (state == NULL) {
        fprintf(stderr, "[form_state] Error: insufficient memory for alloc the form state\n");
        exit(EXIT_FAILURE);
    }

    bool *states = calloc(STATE_COUNT, sizeof(bool)); 
    if (states == NULL) {
        fprintf(stderr, "[form_state] Error: insufficient memory for alloc the form states array\n");
        free(state); 
        exit(EXIT_FAILURE);
    }

    state->states = states; 
    return (FormState) state;
}

void form_state_set(FormState state, FormStatesTypes type, bool status) {
    assert(state);
    assert(type); 

    FormState_st *form_state = (FormState_st *) state; 
    if (form_state == NULL) return; 

    form_state->states[type] = status; 
}

bool form_state_get(FormState state, FormStatesTypes type) {
    assert(state); 
    assert(type);

    FormState_st *form_state = (FormState_st *) state; 
    if (form_state == NULL) return false; 

    return form_state->states[type];
}

void free_form_state(FormState state) {
    assert(state);

    FormState_st *form_state = (FormState_st *) state; 
    if (form_state == NULL) return; 

    free(form_state->states); 
    free(form_state);
}