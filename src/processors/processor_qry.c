#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "processor_qry.h"
#include "../utils/file.h"
#include "../data_structures/list.h"

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

static void o_find_adrees_vertex(char *line_buffer) {

}

static void qry_execute(FILE *qry_file, FILE *txt_file, Graph graph) {
    assert(qry_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 

    while (fgets(line_buffer, sizeof(line_buffer), qry_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue; 

        if (strcmp(command_type, "@o?") == 0) {
            printf("@o?\n");
            o_find_adrees_vertex(line_buffer);
        } else {
            fprintf(stderr, "ERROR: processor_qry unknown command type '%s'\n", command_type);
        }
    }
}

void qry_process(Dir qry, Dir txt, Hash registers, Graph graph) {
    char *file_extension = get_dir_file_extension(qry);
    if (strcmp(file_extension, "qry") != 0) {
        fprintf(stderr, "ERROR: processor_qry requires a .qry file extension\n"); 
        return; 
    }

    if (txt == NULL) {
        fprintf(stderr, "ERROR: processor_qry requires a .txt file to reports\n"); 
        return; 
    }

    FILE *qry_file = file_open_readable(qry);
    if (qry_file == NULL) {
        fprintf(stderr, "ERROR: processor_qry couldn't open the .qry file\n"); 
        return; 
    }
    
    FILE *txt_file = file_open_writable(txt);
    if (txt_file == NULL) {
        fprintf(stderr, "ERROR: processor_qry couldn't open the .txt file\n"); 
        file_close(qry_file);
        return; 
    }

    qry_execute(qry_file, txt_file, graph);
    file_close(qry_file);
    file_close(txt_file);
}