#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "processor_qry.h"
#include "../utils/file.h"
#include "../city/block.h"
#include "../city/point.h"
#include "../data_structures/list.h"

#define MAX_LINE_LENGTH 512 
#define ARG_SIZE 40

static bool process_command(char *line_buffer, char *command_type) {
    if (sscanf(line_buffer, "%9s", command_type) == 0) {
        if (line_buffer[0] != '\n' && line_buffer[0] != '\0') {
            fprintf(stderr, "ERROR: processor_qry unknown line type\n"); 
        } 
        return false;
    }; 

    return true; 
}

struct find_nearest_point_st {
    DescritorTipoInfo nearest_info; 
    Point nearest_point;
    Point target_point; 
    SmuTreap treap; 
};

static void find_nearest_point(void *value, callback_data call_data) {
    SmuNode actual_node = (SmuNode) value; 
    struct find_nearest_point_st *data = (struct find_nearest_point_st *) call_data; 
    
    double x, y, x2, y2; 
    DescritorTipoInfo actual_node_info = (DescritorTipoInfo) getTypeInfoSmuT(data->treap, actual_node);
    Point actual_point = getInfoSmuT(data->treap, actual_node);
    
    assert(actual_point);

    point_get_coordinates(actual_point, &x, &y);
    point_get_coordinates(data->target_point, &x2, &y2);

    double actual_x = 0, actual_y = 0; 
    double actual_distance = -1;
    
    if (data->nearest_point != NULL) {
        point_get_coordinates(data->nearest_point, &actual_x, &actual_y);
        double actual_distance =  sqrt(pow(x2 - actual_x, 2) + pow(y2 - actual_y, 2)); 
    }

    double distance = sqrt(pow(x2 - x, 2) + pow(y2 - y, 2)); 
    if (actual_distance < 0 || distance < actual_distance) {
        data->nearest_info = actual_node_info;
        data->nearest_point = actual_point;
    } 
}

static void o_find_adrees_vertex(char *line_buffer, SmuTreap aux_treap, Hash blocks_table, Hash registers) {
    if (aux_treap == NULL) {
        fprintf(stderr, "(processor_qry) Error: aux treap is not defined\n");
        return;
    }

    char reg[ARG_SIZE] = {0}, block[ARG_SIZE] = {0}, face; 
    int number; 
    int parsed = sscanf(line_buffer, "%*s %s %s %c %d", reg, block, &face, &number); 
    if (parsed != 4) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse command for block find\n"); 
        return; 
    }
    
    Block block_instance = (Block) hash_get(blocks_table, block);
    if (block_instance == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't find block (%s)", block);
        return; 
    }

    int x, y; 
    int range = 50; 
    block_get_adress_coordinate(block_instance, face, number, &x, &y);
    List blocks_in_range_list = new_list(); 
    getNodesDentroRegiaoSmuT(aux_treap, x - range, y - range, x + range, y + range,blocks_in_range_list);

    if (list_get_size(blocks_in_range_list) <= 0) {
        fprintf(stderr, "(processor_qry) Error: couldn't find any vertex within adress range\n");
        return; 
    }

    struct find_nearest_point_st data = {
        .nearest_info = -1, 
        .nearest_point = NULL, 
        .target_point = new_point(x, y),
        .treap = aux_treap
    };
    list_foreach(blocks_in_range_list, &find_nearest_point, &data);

    if (data.nearest_point == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't determine wich vertex is near of the address\n");
        return;
    }

    hash_insert(registers, reg, &data.nearest_info);   
    list_free(blocks_in_range_list, NULL);
}

static void qry_execute(FILE *qry_file, FILE *txt_file, Graph graph, SmuTreap aux_treap, Hash block_table, Hash registers) {
    assert(qry_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 

    while (fgets(line_buffer, sizeof(line_buffer), qry_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue; 

        if (strcmp(command_type, "@o?") == 0) {
            o_find_adrees_vertex(line_buffer, aux_treap, block_table, registers);
        } else {
            fprintf(stderr, "ERROR: processor_qry unknown command type '%s'\n", command_type);
        }
    }
}

void qry_process(Dir qry, Dir txt, Hash registers, Graph graph, SmuTreap aux_treap, Hash blocks_table) {
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

    qry_execute(qry_file, txt_file, graph, aux_treap, blocks_table, registers);
    file_close(qry_file);
    file_close(txt_file);
}