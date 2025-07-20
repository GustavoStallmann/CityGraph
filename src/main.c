#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "city/block.h"
#include "data_structures/list.h"
#include "forms/form.h"
#include "processors/processor_args.h"
#include "processors/processor_dir.h"
#include "processors/processor_geo.h"
#include "processors/processor_svg.h"

typedef struct ProgramConfig_st {
    char *base_input_dir; 
    char *base_output_dir; 
    char *geo_file;
} ProgramConfig_st; 

static ArgManager check_args(int argc, char *argv[]);
static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm);
static void list_extract_block_form(void *value, callback_data call_data);


int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);
    ProgramConfig_st config; 
    extract_config_from_args(&config, argm); 
    
    printf("[CityGraph] -> started\n");
    
    // Read geo file (blocks)
    Dir geo_dir = dir_combine_path_and_file(config.base_input_dir, config.geo_file);
    List blocks = geo_process(geo_dir);
    
    // Extract forms from the blocks
    List block_forms = new_list(); 
    list_foreach(blocks, &list_extract_block_form, block_forms); 
    
    // Export SVG
    Dir svg_directory = dir_combine_path_and_file(config.base_output_dir, "output.svg");
    svg_exporter_init(); 
    svg_set_directory(svg_directory);
    svg_add_forms(block_forms);
    svg_export_forms(); 
    
    // Free memory
    list_free(blocks, NULL); 
    dir_free(geo_dir); 
    free_arg_manager();
    
    return EXIT_SUCCESS;
}

static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm) {
    get_arg_value_by_particle(argm, "-e", &config->base_input_dir);
    get_arg_value_by_particle(argm, "-o", &config->base_output_dir);
    get_arg_value_by_particle(argm, "-f", &config->geo_file);
}

static ArgManager check_args(int argc, char *argv[]) {
    ArgManager argm = new_arg_manager(); 
    add_new_arg(argm, "-e", false, "Diretorio base de entrada (BED)", STR, "./");
    add_new_arg(argm, "-f", true, "Arquivo .geo (deve estar sob o diretorio BED)", STR, "");
    add_new_arg(argm, "-o", true, "Diretorio base de saida (BSD)", STR, "./");
    add_new_arg(argm, "-q", false, "Arquivo .qry para consultas (deve estar sob o diretorio BSD)", STR, "./");
    add_new_arg(argm, "-v", false, "Arquivo .via para construcao do grafo (deve estar sob o diretorio BED)", STR, "./");
    
    bool args_passed = verify_args(argm, argc, argv); 
    if (!args_passed) {
        free_arg_manager(); 
        exit(EXIT_FAILURE);
    }

    return argm; 
}

static void list_extract_block_form(void *value, callback_data call_data) {
    Block block = (Block) value; 
    List target_list = (List) call_data; 

    Form form = block_get_form(block); 
    list_insert(target_list, form);
}