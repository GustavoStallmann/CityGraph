#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "processor_dir.h"
#include "../data_structures/list.h" 
#include "../forms/form_style.h" 
#include "../utils/file.h"

#define MAX_LINE_LENGTH 512 

static bool process_command(char *line_buffer, char *command_type) {
    if (sscanf(line_buffer, "%9s", command_type) == 0) {
        if (line_buffer[0] != '\n' && line_buffer[0] != '\0') {
            fprintf(stderr, "ERROR: processor_qry unknown line type\n"); 
        } 
        return false;
    }; 

    return true; 
}

static List geo_execute(FILE *geo_file) {
    assert(geo_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 

    List form_list = new_list();
    if (form_list == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't create a new list\n"); 
        return NULL; 
    }

    FormStyle actual_font_style = new_form_style("#ffffff", "#ffffff", "Arial", "normal", "start", "12px", "1");
    while (fgets(line_buffer, sizeof(line_buffer), geo_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue;

        // FormInfo form = process_form(command_type, line_buffer, &actual_font_style); 
        // if (form == NULL && strcmp(command_type, "ts") != 0) {
        //     fprintf(stderr, "ERROR: processor_geo couldn't process the form %s\n", command_type); 
        //     continue; 
        // }

        // if (form == NULL) continue; 
        
        // if (list_insert_end(form_list, form) == NULL) {
        //     fprintf(stderr, "ERROR: processor_geo couldn't add the form to the list\n"); 
        //     free_form_info(form);
        // }	
    }

    free_form_style(actual_font_style);
    return form_list;
}

List geo_process(Dir dir) {
    char *file_extension = get_dir_file_extension(dir);
    if (strcmp(file_extension, "geo") != 0) {
        fprintf(stderr, "ERROR: processor_geo requires a .geo file extension\n"); 
        return NULL; 
    }

    FILE *geo_file = file_open_readable(dir);
    if (geo_file == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't open the geo file\n"); 
        return NULL; 
    }

    List form_list = geo_execute(geo_file);     
    file_close(geo_file);
    return form_list; 
}