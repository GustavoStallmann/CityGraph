#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "processor_geo.h"
#include "../data_structures/list.h" 
#include "../forms/form_style.h" 
#include "../city/block.h"
#include "../utils/file.h"

#define MAX_LINE_LENGTH 512 
#define ARG_SIZE 20

static bool process_command(char *line_buffer, char *command_type) {
    if (sscanf(line_buffer, "%9s", command_type) == 0) {
        if (line_buffer[0] != '\n' && line_buffer[0] != '\0') {
            fprintf(stderr, "ERROR: processor_qry unknown line type\n"); 
        } 
        return false;
    }; 

    return true; 
}

static FormStyle process_cq(char *line_buffer) {
    char border_size[ARG_SIZE] = {0}, fill_color[ARG_SIZE] = {0}, border_color[ARG_SIZE] = {0};
    int parsed = sscanf(line_buffer, "%*s %s %s %s", border_size, fill_color, border_color);
    if (parsed != 3) {
        fprintf(stderr, "(processor_geo) Error: couldn't parse cq parameters from line: %s\n", line_buffer);
        return NULL;
    }

    FormStyle ts = new_form_style(border_color, fill_color, NULL, NULL, NULL, NULL, border_size);
    if (ts == NULL) {
        fprintf(stderr, "(processor_geo) Error: couldn't create cq style");
        return NULL; 
    }
    return ts; 
}

static Block process_block(char *line_buffer, FormStyle style, Hash h_table) {
    char block_name[ARG_SIZE] = {0};
    double x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f; 
    int parsed = sscanf(line_buffer, "%*s %s %lf %lf %lf %lf", block_name, &x, &y, &w, &h); 
    if (parsed != 5) {
        fprintf(stderr, "(processor_geo) Error: couldn't parse block parameter from line: %s\n", line_buffer);
        return NULL; 
    }

    FormStyle form_style = new_form_style(
        get_form_style_border_color(style), get_form_style_fill_color(style), 
        NULL, NULL, NULL, NULL, get_form_style_stroke_width(style)
    );
    Form rect = new_form(RECT, -1, x, y, w, h, NULL, form_style); 
    Block block = new_block(block_name, rect); 
    if (rect == NULL || block == NULL || form_style == NULL) {
        fprintf(stderr, "(processor_geo) Error: couldn't create block from line %s\n", line_buffer);
        return NULL;
    }

    hash_insert(h_table, block_name, (HashValue) block);

    return block; 
}

static List geo_process_commands(FILE *geo_file, Hash h_table) {
    assert(geo_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 

    List form_list = new_list();
    if (form_list == NULL) {
        fprintf(stderr, "ERROR: processor_geo couldn't create a new list\n"); 
        return NULL; 
    }

    FormStyle actual_block_style = new_form_style("#ffffff", "#ffffff", NULL, NULL, NULL, NULL, "1px");
    while (fgets(line_buffer, sizeof(line_buffer), geo_file) != NULL) {
        if (!process_command(line_buffer, command_type)) continue;

        Block block = NULL; 
        if (strcmp(command_type, "cq" ) == 0) {
            if (actual_block_style != NULL) {
                free_form_style(actual_block_style);
            }
            actual_block_style = process_cq(line_buffer);
        } else if (strcmp(command_type, "q" ) == 0) {
            block = process_block(line_buffer, actual_block_style, h_table);
        } else {
            fprintf(stderr, "(processor_geo) Error: invalid command on line %s\n", line_buffer);
        }

        if (block == NULL) continue;

        if (list_insert_end(form_list, block) == NULL) {
            fprintf(stderr, "ERROR: processor_geo couldn't add the form to the list\n"); 
            block_free(block);
        }	
    }

    free_form_style(actual_block_style);
    return form_list;
}

List geo_process(Dir dir, Hash h_table) {
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

    List form_list = geo_process_commands(geo_file, h_table);     
    file_close(geo_file);
    return form_list; 
}