#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>

typedef enum {
    DIR, 
    STR, 
    DOUBLE,
    INT
} ArgType; 

typedef void *ArgManager;
typedef void *Arg; 

/* 
 * Create a new arg manager
 * @return the new arg manager
*/
ArgManager new_arg_manager();

/* 
 * Add a new arg to the given arg manager
 * @param argm the argManager to add the new arg
 * @param particle new arg particle 
 * @param mandatory specifies if the arg is mandatory
 * @param arg_description the new arg description
 * @param argType the type of the new argument
 * @param defaultValue the default value for the arg if it got ommited, ignored if mandatory
 * @return ArgManager the given argManager
*/
ArgManager add_new_arg(ArgManager argm, char *particle, bool mandatory, char *arg_description, ArgType argType, void *defaultValue);

/* 
 * Gets the assigned value for the arg by its particle
 * @param argm the argManager to get the arg values from
 * @param particle the particle to search the values
 * @param value the variable to assign the arg value
 * @return the argumentType of the arg
*/
ArgType get_arg_value_by_particle(ArgManager argm, char *particle, void *value); 

/* 
 * Assign and check mandatory args from the program input
 * @param argm the argument manager
 * @param arc the arg count
 * @param arv the arguments array
 * @return bool true if check passed, false otherwise
*/
bool verify_args(ArgManager argm, int argc, char *argv[]);

/* 
 * Free arg manager
*/
void free_arg_manager();

#endif