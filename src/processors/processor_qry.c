#include <complex.h>
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
#include "../city/street.h"
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

static void export_register_identifier(List form_list, double x, double y, char *reg_name) {
    Form text = new_form(TEXT, -1, x, 0, 0, 0, reg_name, NULL);
    Form line = new_form(LINE, -1, x, y, x, 0, NULL, NULL);
    
    list_insert_end(form_list, text);
    list_insert_end(form_list, line);
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

    double current_distance = sqrt(pow(x2 - x, 2) + pow(y2 - y, 2)); 
    
    if (data->nearest_point == NULL) {
        data->nearest_info = actual_node_info;
        data->nearest_point = actual_point;
    } else {
        double actual_x, actual_y; 
        point_get_coordinates(data->nearest_point, &actual_x, &actual_y);
        double nearest_distance = sqrt(pow(x2 - actual_x, 2) + pow(y2 - actual_y, 2)); 
        
        if (current_distance < nearest_distance) {
            data->nearest_info = actual_node_info;
            data->nearest_point = actual_point;
        }
    }
}

static void o_find_adrees_vertex(char *line_buffer, SmuTreap aux_treap, Hash blocks_table, Hash registers, FILE *txt_file) {
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

    fprintf(txt_file, "[*] @o? %s %s %c %d\n", reg, block, face, number);
    
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

    double nearest_x, nearest_y; 
    point_get_coordinates(data.nearest_point, &nearest_x, &nearest_y);
    fprintf(txt_file, "\tPonto encontrado: (%lf, %lf)", nearest_x, nearest_y);

    Node *node_id_ptr = (Node *) malloc(sizeof(Node));
    *node_id_ptr = data.nearest_info;
    hash_insert(registers, reg, node_id_ptr);   
    list_free(blocks_in_range_list, NULL);
}

static void create_path(char *line_buffer, Hash registers, Graph graph, Hash path_table, List forms_export, FILE *txt_file) {
    char path_name[ARG_SIZE] = {0}, subgraph[ARG_SIZE] = {0}, origin[ARG_SIZE] = {0}, dest[ARG_SIZE] = {0}; 
    int parsed = sscanf(line_buffer, "%*s %s %s %s %s", path_name, subgraph, origin, dest); 
    if (parsed != 4) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse the arguments for path creation\n");
        return; 
    }

    fprintf(txt_file, "[*] p? %s %s %s %s\n", path_name, subgraph, origin, dest);
    
    Node *origin_node = (Node *) hash_get(registers, origin);
    Node *dest_node = (Node *) hash_get(registers, dest);

    if (origin_node == NULL || dest_node == NULL) {
        fprintf(stderr, "(processor_qry) Error: origin or dest_node could not be defined\n");
        return; 
    }
    
    Point start_point = getNodeInfo(graph, *origin_node);
    Point end_point = getNodeInfo(graph, *dest_node);
    if (start_point == NULL || end_point == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't get start point from graph\n");
        return;
    }
    
    double start_x, start_y, end_x, end_y; 
    point_get_coordinates(start_point, &start_x, &start_y);
    point_get_coordinates(end_point, &end_x, &end_y);
    
    Dict came_from_length, cost_so_far_length; 
    a_star(graph, *origin_node, *dest_node, &came_from_length, &cost_so_far_length, true);
    List path_length = reconstruct_path(came_from_length, *origin_node, *dest_node, graph, txt_file); 
    
    if (path_length == NULL || list_get_size(path_length) == 0) {
        fprintf(stderr, "(processor_qry) Warning: path_length is empty, no path found\n");
        fprintf(txt_file, "\tNenhum caminho encontrado (ponto inicial: %d; ponto final: %d) \n", *origin_node, *dest_node);
        FormStyle line_style = new_form_style("#FF0000", "#FF0000", NULL, NULL, NULL, NULL, NULL);
        Form line = new_form(LINE, -1, start_x, start_y, end_x, end_y, NULL, line_style);
        list_insert(forms_export, line);
        path_length = new_list();
    }
    
    Form path_form_length = new_animated_form_wrapper(0, start_x, start_y, 10, path_length);
    if (path_form_length == NULL) {
        fprintf(stderr, "(processor_qry) Error: failed to create animated form for length path\n");
        return;
    }
    hash_set(path_table, path_name, path_form_length);
    
    Dict came_from_velocity, cost_so_far_velocity; 
    a_star(graph, *origin_node, *dest_node, &came_from_velocity, &cost_so_far_velocity, false);
    List path_velocity = reconstruct_path(came_from_velocity, *origin_node, *dest_node, graph, txt_file); 
    
    if (path_velocity == NULL || list_get_size(path_velocity) == 0) {
        fprintf(stderr, "(processor_qry) Warning: path_velocity is empty, no path found\n");
        path_velocity = new_list();
    }
    
    Form path_form_velocity = new_animated_form_wrapper(0, start_x, start_y, 10, path_velocity);
    if (path_form_velocity == NULL) {
        fprintf(stderr, "(processor_qry) Error: failed to create animated form for velocity path\n");
        return;
    }
    
    char faster_path_name[ARG_SIZE + 10] = {0};
    snprintf(faster_path_name, sizeof(faster_path_name), "%s_fast", path_name);
    hash_set(path_table, faster_path_name, path_form_velocity);
    export_register_identifier(forms_export, start_x, start_y, faster_path_name);
    export_register_identifier(forms_export, end_x, end_y, faster_path_name);
}

static void output_path(char *line_buffer, Graph graph, Hash paths_table, List forms_to_output, FILE *txt_file) {
    char path_name[ARG_SIZE] = {0}, cmc_color[ARG_SIZE] = {0}, cmr_color[ARG_SIZE] = {0}; 
    int parsed = sscanf(line_buffer, "%*s %s %s %s", path_name, cmc_color, cmr_color); 
    if (parsed != 3) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse arguments for path output\n");
        return;
    }

    fprintf(txt_file, "[*] shw %s %s %s\n", path_name, cmc_color, cmr_color);

    char faster_path_name[ARG_SIZE + 10] = {0};
    snprintf(faster_path_name, sizeof(faster_path_name), "%s_fast", path_name);

    Form path = hash_get(paths_table, path_name);
    Form faster_path = hash_get(paths_table, faster_path_name);
    if (path == NULL || faster_path == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't find paths to output\n");
        return; 
    }

    // Apply colors to differentiate the paths
    FormStyle path_style = form_get_style(path);
    FormStyle faster_path_style = form_get_style(faster_path);
    
    if (path_style != NULL) {
        set_form_border_color(path_style, cmc_color);  // Shortest path color
    }
    
    if (faster_path_style != NULL) {
        set_form_border_color(faster_path_style, cmr_color);  // Fastest path color
    }

    list_insert(forms_to_output, path);
    list_insert(forms_to_output, faster_path);
}

struct disable_street_st {
    SmuTreap treap; 
    Graph graph; 
    List adjacent_edges; 
    FILE *txt_file;
};

static void copy_edge_to_main_list(ListValue value, void *d) {
    struct disable_street_st *data = (struct disable_street_st *) d;
    list_insert(data->adjacent_edges, value);
}

static void get_adjacent_edges(ListValue value, void *d) {
    SmuNode smu_node = (SmuNode *) value; 
    struct disable_street_st *data = (struct disable_street_st *) d; 
    
    Node graph_node = getTypeInfoSmuT(data->treap, smu_node);
    List temp_edges = new_list();
    adjacentEdges(data->graph, graph_node, temp_edges);
    
    list_foreach(temp_edges, &copy_edge_to_main_list, data);
    list_free(temp_edges, NULL);
}

static void disable_edges(ListValue value, void *d) {
    Edge edge = (Edge) value; 
    struct disable_street_st *data = (struct disable_street_st *) d; 

    Street edge_street = getEdgeInfo(data->graph, edge);
    street_set_status(edge_street, false);

    double mean_vel, length; 
    char street_name[50];
    
    street_get_specs(edge_street, &mean_vel, &length, NULL, street_name);
    fprintf(data->txt_file, "\tdesabilitando aresta de nome: %s (velocidade media: %lf; comprimento: %lf)\n", street_name, mean_vel, length);
}

static void alag(char *line_buffer, Dict alags, SmuTreap treap, Graph graph, List export_forms, FILE *txt_file) {
    int alag_n; 
    double x, y, w, h; 
    int parsed = sscanf(line_buffer, "%*s %d %lf %lf %lf %lf", &alag_n, &x, &y, &w, &h);
    if (parsed != 5) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse alag command\n");
        return; 
    }
    
    fprintf(txt_file, "[*] alag %d %lf %lf %lf %lf\n",  alag_n, x, y, w, h);

    FormStyle rect_style = new_form_style("#AA0044", "#AB37C8", NULL, NULL, NULL, NULL, NULL);
    Form alag_rect = new_form(RECT, -1, x, y, w, h, NULL, rect_style);
    list_insert(export_forms, alag_rect);
    
    List nodes_in_area = new_list(); 
    List adjacent_edges = new_list();
    getNodesDentroRegiaoSmuT(treap, x, y, x + w, y + h, nodes_in_area);

    struct disable_street_st data = {
        .treap = treap, 
        .graph = graph,
        .adjacent_edges = adjacent_edges,
        .txt_file = txt_file
    };
    list_foreach(nodes_in_area, &get_adjacent_edges, &data);
    list_foreach(adjacent_edges, &disable_edges, &data);

    dict_put(alags, alag_n, adjacent_edges);
    list_free(nodes_in_area, NULL);
}


struct dren_data_st { 
    Graph graph; 
    FILE *txt_file;
};
static void enable_edge(ListValue value, void *d) {
    Edge edge = (Edge) value; 
    struct dren_data_st *data = (struct dren_data_st *) d; 

    Street edge_street = getEdgeInfo(data->graph, edge);
    street_set_status(edge_street, true);

    double mean_vel, length; 
    char street_name[50];
    street_get_specs(edge_street, &mean_vel, &length, NULL, street_name);
    fprintf(data->txt_file, "\t habilitando aresta de nome: %s (velocidade media: %lf; comprimento: %lf)\n", street_name, mean_vel, length);
}

static void dren(char *line_buffer, Dict alags, Graph graph, FILE *txt_file) {
    int alag_n; 
    int parsed = sscanf(line_buffer, "%*s %d", &alag_n);
    if (parsed != 1) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse dren command\n");
        return; 
    }

    fprintf(txt_file, "[*] dren %d\n",  alag_n);

    List edges_list = dict_get(alags, alag_n);
    if (edges_list == NULL) {
        fprintf(stderr, "(processor_qry) Error: alag %d not found\n", alag_n);
        return;
    }

    struct dren_data_st data = {
        .graph = graph,
        .txt_file = txt_file
    };
    list_foreach(edges_list, &enable_edge, &data);
}

static void join(char *line_buffer, Graph graph, Hash paths, SmuTreap aux_treap, FILE *txt_file, List forms_export) {
    char np[ARG_SIZE] = {0}, np1[ARG_SIZE] = {0}, np2[ARG_SIZE] = {0};
    int parsed = sscanf(line_buffer, "%*s %s %s %s", np, np1, np2);
    if (parsed != 3) {
        fprintf(stderr, "(processor_qry) Error: couldn't parse join args\n");
        return; 
    }

    fprintf(txt_file, "[*] join %s %s %s\n", np, np1, np2);

    Form np1_form = hash_get(paths, np1);
    Form np2_form = hash_get(paths, np2);
    
    if (np1_form == NULL || np2_form == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't find the np1 or np2 paths\n");
        fprintf(txt_file, "\tErro: não foi possível encontrar os caminhos %s ou %s\n", np1, np2);
        return; 
    }

    List np1_points = form_get_path_points(np1_form);
    List np2_points = form_get_path_points(np2_form);
    if (np1_points == NULL || np2_points == NULL) {
        fprintf(stderr, "(processor_qry) Error: couldn't get path points from forms\n");
        return;
    }

    int np1_size = list_get_size(np1_points);
    int np2_size = list_get_size(np2_points);
    if (np1_size == 0 || np2_size == 0) {
        fprintf(stderr, "(processor_qry) Error: one of the paths is empty\n");
        return;
    }

    Point np1_end_point = list_get(np1_points, np1_size-1); 
    Point np2_start_point = list_get(np2_points, 0);

    
    double np1_x, np1_y, np2_x, np2_y;
    point_get_coordinates(np1_end_point, &np1_x, &np1_y);
    point_get_coordinates(np2_start_point, &np2_x, &np2_y);
    
    double search_radius = 1.0;
    
    List np1_nearby_nodes = new_list();
    List np2_nearby_nodes = new_list();
    
    getNodesDentroRegiaoSmuT(aux_treap, np1_x - search_radius, np1_y - search_radius, 
                             np1_x + search_radius, np1_y + search_radius, np1_nearby_nodes);
    getNodesDentroRegiaoSmuT(aux_treap, np2_x - search_radius, np2_y - search_radius, 
                             np2_x + search_radius, np2_y + search_radius, np2_nearby_nodes);
    
    if (list_get_size(np1_nearby_nodes) == 0 || list_get_size(np2_nearby_nodes) == 0) {
        fprintf(stderr, "(processor_qry) Error: no nodes found near join points\n");
        fprintf(txt_file, "\tErro: não foram encontrados nós próximos aos pontos de junção\n");
        list_free(np1_nearby_nodes, NULL);
        list_free(np2_nearby_nodes, NULL);
        return;
    }
    
    struct find_nearest_point_st np1_data = {
        .nearest_info = -1, 
        .nearest_point = NULL, 
        .target_point = np1_end_point,
        .treap = aux_treap
    };
    
    struct find_nearest_point_st np2_data = {
        .nearest_info = -1, 
        .nearest_point = NULL, 
        .target_point = np2_start_point,
        .treap = aux_treap
    };
    
    list_foreach(np1_nearby_nodes, &find_nearest_point, &np1_data);
    list_foreach(np2_nearby_nodes, &find_nearest_point, &np2_data);
    
    if (np1_data.nearest_info == -1 || np2_data.nearest_info == -1) {
        fprintf(stderr, "(processor_qry) Error: couldn't find nearest nodes for join points\n");
        fprintf(txt_file, "\tErro: não foi possível encontrar os nós mais próximos para a junção\n");
        list_free(np1_nearby_nodes, NULL);
        list_free(np2_nearby_nodes, NULL);
        return;
    }
    
    Node np1_end_node = np1_data.nearest_info;
    Node np2_start_node = np2_data.nearest_info;
    
    list_free(np1_nearby_nodes, NULL);
    list_free(np2_nearby_nodes, NULL);

    Dict came_from_length, cost_so_far_length; 
    a_star(graph, np1_end_node, np2_start_node, &came_from_length, &cost_so_far_length, true);
    List connecting_path_length = reconstruct_path(came_from_length, np1_end_node, np2_start_node, graph, txt_file); 
    
    if (connecting_path_length == NULL || list_get_size(connecting_path_length) == 0) {
        fprintf(stderr, "(processor_qry) Warning: no connecting path found between np1 end and np2 start\n");
        fprintf(txt_file, "\tNenhum caminho de conexão encontrado entre final de %s e início de %s\n", np1, np2);
        connecting_path_length = new_list();
    }
    
    Form connecting_path_form = new_animated_form_wrapper(0, np1_x, np1_y, 10, connecting_path_length);
    if (connecting_path_form == NULL) {
        fprintf(stderr, "(processor_qry) Error: failed to create animated form for connecting path\n");
        return;
    }

    hash_set(paths, np, connecting_path_form);

    Dict came_from_velocity, cost_so_far_velocity; 
    a_star(graph, np1_end_node, np2_start_node, &came_from_velocity, &cost_so_far_velocity, false);
    List connecting_path_velocity = reconstruct_path(came_from_velocity, np1_end_node, np2_start_node, graph, txt_file); 
    
    if (connecting_path_velocity == NULL || list_get_size(connecting_path_velocity) == 0) {
        connecting_path_velocity = new_list();
    }
    
    Form connecting_path_form_velocity = new_animated_form_wrapper(0, np1_x, np1_y, 10, connecting_path_velocity);
    if (connecting_path_form_velocity != NULL) {
        char np_fast_name[ARG_SIZE + 10];
        snprintf(np_fast_name, sizeof(np_fast_name), "%s_fast", np);
        hash_set(paths, np_fast_name, connecting_path_form_velocity);
    }

    fprintf(txt_file, "\tPercurso %s criado conectando final de %s com início de %s\n", np, np1, np2);
}

static void qry_execute(FILE *qry_file, FILE *txt_file, Graph graph, SmuTreap aux_treap, Hash block_table, Hash registers, List export_forms) {
    assert(qry_file);

    char line_buffer[MAX_LINE_LENGTH]; 
    char command_type[10]; 
    Hash paths = new_hash(10); 
    Dict alags = new_dict(100);

    while (fgets(line_buffer, sizeof(line_buffer), qry_file) != NULL) {
        if (process_command(line_buffer, command_type) == false) continue; 

        if (strcmp(command_type, "@o?") == 0) {
            o_find_adrees_vertex(line_buffer, aux_treap, block_table, registers, txt_file);
        } else if (strcmp(command_type, "p?") == 0) {
            create_path(line_buffer, registers, graph, paths, export_forms, txt_file);
        } else if (strcmp(command_type, "shw") == 0) {
            output_path(line_buffer, graph, paths, export_forms, txt_file);
        } else if (strcmp(command_type, "alag") == 0) {
            alag(line_buffer, alags, aux_treap, graph, export_forms, txt_file);
        } else if (strcmp(command_type, "dren") == 0) {
            dren(line_buffer, alags, graph, txt_file);
        } else if (strcmp(command_type, "join") == 0) {
            join(line_buffer, graph, paths, aux_treap, txt_file, export_forms);
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