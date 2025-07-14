#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "data_structures/list.h"
#include "processors/processor_args.h"
#include "processors/processor_dir.h"
#include "processors/processor_geo.h"

typedef struct ProgramConfig_st {
    char *base_input_dir; 
    char *geo_file;
} ProgramConfig_st; 

static ArgManager check_args(int argc, char *argv[]);
static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm);

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);
    ProgramConfig_st config; 
    extract_config_from_args(&config, argm); 

    printf("[CityGraph] -> started\n");

    List forms = geo_process(config.geo_file);
    
    free_arg_manager();
    return EXIT_SUCCESS;
}

static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm) {
    get_arg_value_by_particle(argm, "-e", config->base_input_dir);
    get_arg_value_by_particle(argm, "-f", config->geo_file);
}

static ArgManager check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager(); 
    add_new_arg(argm, "-e", false, "Diretorio base de entrada (BED)", DIR, "./");
    add_new_arg(argm, "-f", true, "Arquivo .geo (deve estar sob o diretorio BED)", DIR, "./");
    add_new_arg(argm, "-o", true, "Diretorio base de saida (BSD)", DIR, "./");
    add_new_arg(argm, "-q", false, "Arquivo .qry para consultas (deve estar sob o diretorio BSD)", DIR, "./");
    add_new_arg(argm, "-v", false, "Arquivo .via para construcao do grafo (deve estar sob o diretorio BED)", DIR, "./");
    bool args_passed = verify_args(argm, argc, argv); 
    if (!args_passed) {
        free_arg_manager(); 
        exit(EXIT_FAILURE);
    }

    return argm; 
}