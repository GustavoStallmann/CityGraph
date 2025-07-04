#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "processor_args.h"
#include "processor_dir.h"
#include "../data_structures/list.h"

typedef struct {
    char *particle; 
    char *arg_description; 
    bool mandatory; 
    ArgType argType; 
    union {
        char *str; 
        double num_d; 
        Dir dir; 
        int num_i; 
    } value; 
} Arg_st; 

typedef struct {
    List list; 
} ArgManager_st; 

static ArgManager_st *arg_manager = NULL; 

static void alloc_error() {
    fprintf(stderr, "[processor_args] Error: insufficient memory for allocation\n");
    exit(EXIT_FAILURE);     
}

static inline bool is_particle(char *str) {
    return str != NULL? str[0] == '-': false; 
}

static char* alloc_str(char *str) {
    if (str == NULL) return NULL; 
    
    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size); //alloc a new str 
    if (new_str == NULL) alloc_error(); 
    strcpy(new_str, str);
    return new_str; 
}

static void count_mandatory_args(void *a, callback_data d) {
    Arg_st *arg = (Arg_st *) a; 
    int *data = (int *) d; 
    
    if (arg->mandatory) (*data)++; 
}

static int get_total_mandatory_args(ArgManager_st *arg_manager) {
    int count = 0;  
    
    list_foreach(arg_manager->list, 
        &count_mandatory_args, 
        &count
    );
    
    return count; 
}

static bool assign_arg_value(Arg_st *arg, char *value) {
    switch (arg->argType) {
        case DIR: 
            if (arg->value.dir != NULL) {
                dir_free(arg->value.dir);
            }
            arg->value.dir = new_dir(value); 
            if (arg->value.dir == NULL) return false; 
            break; 
        case STR: 
            if (arg->value.str != NULL) {
                free(arg->value.str);
            }
            arg->value.str = alloc_str(value);
            if (arg->value.str == NULL) return false; 
            break; 
        case DOUBLE:
            arg->value.num_d = atof(value);
            break;
        case INT: 
            arg->value.num_i = atoi(value);
            break;
        default:
            fprintf(stderr, "[processor_args] Error: invalid arg type");
            return false; 
    }
    
    return true; 
}

ArgManager new_arg_manager() {
    if (arg_manager == NULL) {
        arg_manager = (ArgManager_st *) malloc(sizeof(ArgManager_st));
        if (arg_manager == NULL) alloc_error(); 
        
        arg_manager->list = new_list(); 
    } 

    return (ArgManager) arg_manager;
}

static bool compare_list_search(ListValue value, void *target) {
    Arg_st *arg = (Arg_st *) value; 
    char *search_particle = (char *) target; 
    if (arg == NULL || arg->particle == NULL || search_particle == NULL) return false; 

    return strcmp(arg->particle, search_particle) == 0;
}

Arg_st* get_arg_by_particle(ArgManager argm, char *particle) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm; 
    if (arg_manager == NULL) return NULL; 
    
    Arg_st *arg = (Arg_st *) list_search(arg_manager->list, (ListValue) particle, &compare_list_search);
    if (arg == NULL) return NULL; 
    
    return arg; 
}

ArgType get_arg_value_by_particle(ArgManager argm, char *particle, void *value) {
    Arg_st *arg = get_arg_by_particle(argm, particle); 
    assert(argm); 

    if (arg == NULL) return -1; 
    
    switch (arg->argType) {
        case DIR: 
            *((Dir *)value) = arg->value.dir; 
            break; 
        case STR:
            *((char **)value) = arg->value.str; 
            break; 
        case DOUBLE:
            *((double *)value) = arg->value.num_d; 
            break; 
        case INT: 
            *((int *)value) = arg->value.num_i;
            break;
        default:
            fprintf(stderr, "[processor_args] Error: couldn't find argument for processor_args");
            return -1; 
    }

    return arg->argType; 
}

ArgManager add_new_arg(ArgManager argm, char *particle, bool mandatory, char *arg_description, ArgType argType, void *defaultValue) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm; 
    if (arg_manager == NULL) return NULL; 
    
    Arg_st *new_arg = (Arg_st *) calloc(1, sizeof(Arg_st));
    if (new_arg == NULL) alloc_error(); 
    
    new_arg->particle = alloc_str(particle);
    new_arg->mandatory = mandatory;
    new_arg->arg_description = alloc_str(arg_description);
    new_arg->argType = argType; 

    if (defaultValue != NULL) assign_arg_value(new_arg, defaultValue); 

    list_insert(arg_manager->list, (ListValue) new_arg); 
    return argm; 
}

static void print_argument(ListValue a, void *_) {
    (void)_; 
    Arg_st *arg = (Arg_st *) a; 
    if (arg == NULL) return; 

    printf("\t%s\t(%s) %s\n", 
        arg->particle, arg->mandatory? "mandatory" : "optional",  arg->arg_description
    ); 
}

static void print_argument_option(ArgManager argm) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm;
    if (arg_manager == NULL) return; 
    list_foreach(arg_manager->list, &print_argument, NULL);
}

bool verify_args(ArgManager argm, int argc, char *argv[]) {
    ArgManager_st *arg_manager = (ArgManager_st *) argm;
    if (arg_manager == NULL) return false; 
    
    int total_mandatories = get_total_mandatory_args(arg_manager); 
    if (total_mandatories > (argc-1)) {
        fprintf(stderr, "[processor_args] Error: Mandatory arguments were omitted\n");
        print_argument_option(arg_manager);
        return false;
    }
    
    for (int i = 1; i < argc; i++) {
        char *actual_arg = argv[i]; 
        if (!is_particle(actual_arg)) continue;

        if (i + 1 >= argc) {
            fprintf(stderr, "[processor_args] Error: an argument must be followed by a value\n");
            return false; 
        };

        char *next_arg = argv[++i]; // jump to the next arg
        if (is_particle(next_arg)) { 
            fprintf(stderr, "[processor_args] Error: Two particles were informed consecutively\n");
            print_argument_option(arg_manager);
            return false; 
        } 

        Arg_st* arg = get_arg_by_particle(argm, actual_arg); 
        if (arg == NULL) { 
            fprintf(stderr, "[processor_args] Error: An invalid argument were informed, available options: \n");
            print_argument_option(arg_manager);
            return false; 
        } 

        if (!assign_arg_value(arg, next_arg)) {
            return false; 
        }
    }
    
    return true; 
}

static void free_arg(ListValue a) {
    Arg_st *arg = (Arg_st *) a; 
    
    free(arg->arg_description); 
    free(arg->particle);
    switch (arg->argType) {
        case DIR:
            dir_free(arg->value.dir);
            break;
        case STR:
            free(arg->value.str);
            break;
        default:
            break;
    }

    free(arg); 
}

void free_arg_manager( ) {
    if (arg_manager == NULL) return; 
    
    list_free(arg_manager->list, &free_arg); 
    free(arg_manager);
    arg_manager = NULL; 
}