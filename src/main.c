#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "processors/processor_args.h"

bool check_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    printf("[CityGraph] -> started\n");

    bool args_passed = check_args(argc, argv);
    if (!args_passed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

bool check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager(); 
    add_new_arg(argm, "-e", false, "Argumento teste", STR, "default");
    bool args_passed = verify_args(argm, argc, argv); 
    free_arg_manager();
    return args_passed; 
}