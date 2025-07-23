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
#include "../city/path.h"
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

    Node *node_id_ptr = (Node *) malloc(sizeof(Node));
    *node_id_ptr = data.nearest_info;
    hash_insert(registers, reg, node_id_ptr);   
    list_free(blocks_in_range_list, NULL);
}

static void create_path(char *line_buffer, Hash registers, Graph graph, Hash path_table) {
    char path_name[ARG_SIZE] = {0}, subgraph[ARG_SIZE] = {0}, origin[ARG_SIZE] = {0}, dest[ARG_SIZE] = {0}; 
    int parsed = sscanf(line_buffer, "%*s %s %s %s %s", path_name, subgraph, origin, dest); 
    if (parsed != 4) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse the arguments for path creation\n");
        return; 
    }
    
    Node *origin_node = (Node *) hash_get(registers, origin);
    Node *dest_node = (Node *) hash_get(registers, dest);

    if (origin_node == NULL || dest_node == NULL) {
        fprintf(stderr, "(processor_qry) Error: origin or dest_node could not be defined\n");
        return; 
    }
    
    Point start_point = getNodeInfo(graph, *origin_node);
    double start_x, start_y; 
    point_get_coordinates(start_point, &start_x, &start_y);
    
    Dict came_from_length, cost_so_far_length; 
    a_star(graph, *origin_node, *dest_node, &came_from_length, &cost_so_far_length, true);
    List path_length = reconstruct_path(came_from_length, *origin_node, *dest_node, graph); 
    Form path_form_length = new_animated_form_wrapper(0, start_x, start_y, 10, path_length);
    hash_set(path_table, path_name, path_form_length);
    
    Dict came_from_velocity, cost_so_far_velocity; 
    a_star(graph, *origin_node, *dest_node, &came_from_velocity, &cost_so_far_velocity, false);
    List path_velocity = reconstruct_path(came_from_velocity, *origin_node, *dest_node, graph); 
    Form path_form_velocity = new_animated_form_wrapper(0, start_x, start_y, 10, path_velocity);
    
    char faster_path_name[ARG_SIZE + 10] = {0};
    snprintf(faster_path_name, sizeof(faster_path_name), "%s_fast", path_name);
    hash_set(path_table, faster_path_name, path_form_velocity);
}

static void output_path(char *line_buffer, Graph graph, Hash paths_table) {
    char path_name[ARG_SIZE] = {0}, cmc_color[ARG_SIZE] = {0}, cmr_color[ARG_SIZE] = {0}; 
    int parsed = sscanf(line_buffer, "%*s %s %s %s", path_name, cmc_color, cmr_color); 
    if (parsed != 3) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse aguments for path output");
    }
    // TODO: OUTPUT PATH 
    // Form path = 
}

static void qry_execute(FILE *qry_file, FILE *txt_file, Graph graph, SmuTreap aux_treap, Hash block_table, Hash registers, List export_forms) {
    assert(qry_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 
    Hash paths = new_hash(10); 

    while (fgets(line_buffer, sizeof(line_buffer), qry_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue; 

        if (strcmp(command_type, "@o?") == 0) {
            o_find_adrees_vertex(line_buffer, aux_treap, block_table, registers);
        } else if (strcmp(command_type, "p?") == 0) {
            create_path(line_buffer, registers, graph, paths);
        } else if (strcmp(command_type, "shw") == 0) {
            
        } else {
            fprintf(stderr, "ERROR: processor_qry unknown command type '%s'\n", command_type);
        }
    }

}

List qry_process(Dir qry, Dir txt, Hash registers, Graph graph, SmuTreap aux_treap, Hash blocks_table) {
    List export_forms = new_list(); 

    char *file_extension = get_dir_file_extension(qry);
    if (strcmp(file_extension, "qry") != 0) {
        fprintf(stderr, "ERROR: processor_qry requires a .qry file extension\n"); 
        return NULL; 
    }

    if (txt == NULL) {
        fprintf(stderr, "ERROR: processor_qry requires a .txt file to reports\n"); 
        return NULL; 
    }

    FILE *qry_file = file_open_readable(qry);
    if (qry_file == NULL) {
        fprintf(stderr, "ERROR: processor_qry couldn't open the .qry file\n"); 
        return NULL; 
    }
    
    FILE *txt_file = file_open_writable(txt);
    if (txt_file == NULL) {
        fprintf(stderr, "ERROR: processor_qry couldn't open the .txt file\n"); 
        file_close(qry_file);
        return NULL; 
    }

    qry_execute(qry_file, txt_file, graph, aux_treap, blocks_table, registers, export_forms);
    file_close(qry_file);
    file_close(txt_file);

    return export_forms; 
}