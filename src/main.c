#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "city/block.h"
#include "data_structures/list.h"
#include "data_structures/hash_table.h"
#include "forms/form.h"
#include "processors/processor_args.h"
#include "processors/processor_dir.h"
#include "processors/processor_geo.h"
#include "processors/processor_qry.h"
#include "processors/processor_svg.h"
#include "processors/processor_vias.h"

typedef struct ProgramConfig_st {
    char *base_input_dir; 
    char *base_output_dir; 
    char *geo_file;
    char *via_file; 
    char *qry_file; 
} ProgramConfig_st; 

static ArgManager check_args(int argc, char *argv[]);
static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm);
static void list_extract_block_form(void *value, callback_data call_data);
static void combine_file_names(char *str1, char *str2, char *file_extension, char *result, int size);

int main(int argc, char *argv[]) {
    ArgManager argm = check_args(argc, argv);
    ProgramConfig_st config; 
    extract_config_from_args(&config, argm); 
    
    printf("[CityGraph] -> started\n");

    // Register
    Hash registers = new_hash(11); 

    // Process geo file (blocks)
    Dir geo_dir = dir_combine_path_and_file(config.base_input_dir, config.geo_file);
    List blocks = geo_process(geo_dir);
    
    // Process .via file (city)
    Dir via_dir = dir_combine_path_and_file(config.base_input_dir, config.via_file); 
    Graph city_graph = via_process(via_dir);
    
    // Process .qry file 
    Dir qry_dir = dir_combine_path_and_file(config.base_input_dir, config.qry_file);
    char txt_file_name[50]; 
    combine_file_names(get_dir_file_name(geo_dir), get_dir_file_name(qry_dir), "txt", txt_file_name, sizeof(txt_file_name));
    Dir txt_dir = dir_combine_path_and_file(config.base_output_dir, txt_file_name);
    qry_process(qry_dir, txt_dir, registers, city_graph);
    
    // Extract forms from the blocks
    List block_forms = new_list(); 
    list_foreach(blocks, &list_extract_block_form, block_forms); 
    
    // Export SVG
    char svg_file_name[50]; 
    combine_file_names(get_dir_file_name(geo_dir), get_dir_file_name(qry_dir), "svg", svg_file_name, sizeof(svg_file_name));
    Dir svg_dir = dir_combine_path_and_file(config.base_output_dir, svg_file_name);
    svg_exporter_init(); 
    svg_set_directory(svg_dir);
    svg_add_forms(block_forms);
    svg_export_forms(); 
    
    // Free memory
    list_free(blocks, NULL); 
    dir_free(geo_dir); 
    free_arg_manager();
    
    return EXIT_SUCCESS;
}

static void combine_file_names(char *str1, char *str2, char *file_extension, char *result, int size) {
    if (str2 != NULL) {
        snprintf(result, size, "%s-%s.%s", str1, str2, file_extension);
        return; 
    }
    snprintf(result, size, "%s.%s", str1, file_extension);
}

static void extract_config_from_args(ProgramConfig_st *config, ArgManager argm) {
    get_arg_value_by_particle(argm, "-e", &config->base_input_dir);
    get_arg_value_by_particle(argm, "-o", &config->base_output_dir);
    get_arg_value_by_particle(argm, "-f", &config->geo_file);
    get_arg_value_by_particle(argm, "-v", &config->via_file);
    get_arg_value_by_particle(argm, "-q", &config->qry_file);
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

    if (block == NULL) {
        fprintf(stderr, "(main) Error: skipping block exportation\n");
        return; 
    }

    Form block_form = block_get_form(block); 
    Form text_form = block_get_form_text(block);

    if (block_form != NULL) {
        list_insert(target_list, block_form);
    }

    if (text_form != NULL) {
        list_insert_end(target_list, text_form);
    }
}