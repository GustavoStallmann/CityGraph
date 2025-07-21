#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "processor_vias.h"
#include "../data_structures/graph.h"
#include "../utils/file.h"
#include "../city/point.h"
#include "../city/street.h"
#include "processor_dir.h"

#define MAX_LINE_LENGTH 512 
#define ARG_SIZE 40

static bool process_command(char *line_buffer, char *command_type) {
    if (sscanf(line_buffer, "%9s", command_type) == 0) {
        if (line_buffer[0] != '\n' && line_buffer[0] != '\0') {
            fprintf(stderr, "(processor_vias) Error: unknown line type\n"); 
        } 
        return false;
    }; 

    return true; 
}

static void process_nv(Graph *graph, char *line_buffer) {
    if (*graph != NULL) {
        fprintf(stderr, "(processor_via) Error: aborting attempt of graph overwrite\n");
        return; 
    }

    int g_size; 
    int parsed = sscanf(line_buffer, "%d", &g_size); 
    if (parsed != 1) {
        fprintf(stderr, "(processor_via) Error: couldn't parse graph size\n"); 
        return; 
    }

    *graph = createGraph(g_size, false, "-"); 
}

static void process_v(char *line_buffer, Graph graph) {
    if (graph == NULL) {
        fprintf(stderr, "(processor_vias) Error: graph's unintialized, skipping vertex creation\n");
        return; 
    }

    char v_name[ARG_SIZE] = {0}; 
    double x, y; 
    int parsed = sscanf(line_buffer, "%*s %s %lf %lf", v_name, &x, &y); 
    if (parsed != 3) {
        fprintf(stderr, "(processor_vias) Error: couldn't parse command for vertex creation\n");
        return; 
    }

    Point v_point = new_point(x, y); 
    addNode(graph, v_name, v_point);
}

static void process_e(char *line_buffer, Graph graph) {
    if (graph == NULL) {
        fprintf(stderr, "(processor_vias) Error: graph's unintialized, skipping edge creation\n");
        return; 
    }

    char node_start[ARG_SIZE] = {0}, node_target[ARG_SIZE] = {0}; 
    char left_block[ARG_SIZE] = {0}, right_block[ARG_SIZE] = {0}; 
    double length, mean_vel; 
    char edge_name[ARG_SIZE] = {0}; 
    
    int parsed = sscanf(line_buffer, "%*s %s %s %s %s %lf %lf %s", 
        node_start, node_target, left_block, right_block, &length, &mean_vel, edge_name
    );
    if (parsed != 7) {
        fprintf(stderr, "(processor_vias) Error: couldn't parse command for edge creation\n"); 
        return; 
    }

    Node from_node = getNode(graph, node_start);
    Node to_node = getNode(graph, node_target); 
    if (from_node < 0 || to_node < 0) {
        fprintf(stderr, "(processor_vias) Error: couldn't found nods by name (%s | %s)\n", node_start, node_target);
        return; 
    }

    Street edge_street = new_street(edge_name);
    addEdge(graph, from_node, to_node, edge_street);
}

static void via_process_commands(FILE *via_file, Graph *graph) {
    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[ARG_SIZE]; 

    bool first_line = true; 
    while (fgets(line_buffer, sizeof(line_buffer), via_file) != NULL) {
        if (first_line) {
            process_nv(graph, line_buffer);
            first_line = false; 
            continue;
        }

        if (!process_command(line_buffer, command_type)) continue; 

        if (*graph == NULL) {
            fprintf(stderr, "(processor_vias) Error: graph wasn't created yet\n");
            continue;
        }

        if (strcmp(command_type, "v") == 0) {
            process_v(line_buffer, *graph);
        } else if (strcmp(command_type, "e") == 0) {
            process_e(line_buffer, *graph); 
        } else {
            fprintf(stderr, "(processor_vias) Error: invalid command type (%s)\n", command_type); 
        }
    }
}

Graph via_process(Dir dir) {
    assert(dir); 

    char *file_extension = get_dir_file_extension(dir); 
    if (strcmp(file_extension, "via") != 0) {
        fprintf(stderr, "(processor_vias) Error: invalid file format (%s)\n", file_extension);
        return NULL; 
    }

    FILE *via_file = file_open_readable(dir); 
    if (via_file == NULL) {
        fprintf(stderr, "(processor_via) Error: couldn't open .via file\n"); 
        return NULL; 
    }

    Graph graph = NULL;
    via_process_commands(via_file, &graph);

    file_close(via_file);
    return graph; 
}