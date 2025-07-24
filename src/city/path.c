#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <stdbool.h>
#include "../data_structures/graph.h"
#include "../data_structures/pqueue.h"
#include "../data_structures/dictionary.h"
#include "street.h"
#include "point.h"
#include "path.h"

typedef struct AStarData_st {
    Dict came_from; 
    Dict cost_so_far; 
    PriorityQueue queue; 
    Node current; 
    Node goal;
    Graph graph; 
    bool use_length;
} AStarData_st; 

static double heuristic(Point x, Point y) {
    double x1, y1, x2, y2; 
    point_get_coordinates(x, &x1, &y1);
    point_get_coordinates(y, &x2, &y2);
    return sqrt(pow(x2 - x1, 2) + pow(y2 -y1, 2));
}

static void a_star_foreach_adjacencie(ListValue value, void *d) {
    Edge edge = (Edge) value; 
    AStarData_st *data = (AStarData_st *) d; 
    if (edge == NULL) return; 
    
    Node to_node = getToNode(data->graph, edge);
    Street edge_info = getEdgeInfo(data->graph, edge);

    double street_mean_velocity = 0, street_length = 0; 
    bool street_enabled = false; 
    street_get_specs(edge_info, &street_mean_velocity, &street_length, &street_enabled, NULL);
    if (!street_enabled) return; 

    double *actual_cost_so_far = (double *) dict_get(data->cost_so_far, data->current); 
    if (actual_cost_so_far == NULL) {
        return;
    } 

    double *new_cost = (double *) malloc(sizeof(double));
    
    if (data->use_length) {
        *new_cost = *actual_cost_so_far + street_length;
    } else {
        *new_cost = *actual_cost_so_far + (street_length / street_mean_velocity);
    } 

    double *next_cost = (double *) dict_get(data->cost_so_far, to_node);
    bool next_not_in_cost_so_far = (next_cost == NULL);

    if (next_not_in_cost_so_far || *new_cost < *next_cost) {
        dict_put(data->cost_so_far, to_node, new_cost); 
        Point to_point = getNodeInfo(data->graph, to_node);
        Point goal_point = getNodeInfo(data->graph, data->goal);

        double priority = *new_cost + heuristic(to_point, goal_point);
        
        Node *to_node_ptr = (Node *) malloc(sizeof(Node));
        *to_node_ptr = to_node;
        pqueue_enqueue(data->queue, to_node_ptr, priority);
        
        Node *from_node_ptr = (Node *) malloc(sizeof(Node));
        *from_node_ptr = data->current;
        dict_put(data->came_from, to_node, from_node_ptr); 
    } else {
        free(new_cost);
    }
}

void a_star(Graph graph, Node start, Node goal, Dict *came_from, Dict *cost_so_far, bool use_length) {
    int graph_size = getMaxNodes(graph);
    if (graph_size <= 0) return; 

    PriorityQueue frontier = new_priority_queue(graph_size); 
    Node *start_ptr = (Node *) malloc(sizeof(Node));
    *start_ptr = start;
    pqueue_enqueue(frontier, start_ptr, 0);

    *came_from = new_dict(graph_size);
    *cost_so_far = new_dict(graph_size);

    dict_put(*came_from, start, NULL);
    
    double *start_cost = (double *) malloc(sizeof(double));
    *start_cost = 0.0;
    dict_put(*cost_so_far, start, start_cost);

    while (!pqueue_is_empty(frontier)) {
        Node *current_node = pqueue_dequeue(frontier);
        if (current_node == NULL) break;

        if (*current_node == goal) {
            free(current_node);
            break; 
        }

        List current_node_adjacencies = new_list(); 
        adjacentEdges(graph, *current_node, current_node_adjacencies); 

        AStarData_st data = {
            .came_from = *came_from, 
            .cost_so_far = *cost_so_far, 
            .graph = graph, 
            .current = *current_node, 
            .goal = goal,
            .queue = frontier,
            .use_length = use_length
        };
        list_foreach(current_node_adjacencies, &a_star_foreach_adjacencie, &data);
        list_free(current_node_adjacencies, NULL);
        free(current_node);
    }
    
    while (!pqueue_is_empty(frontier)) {
        Node *remaining_node = pqueue_dequeue(frontier);
        if (remaining_node != NULL) {
            free(remaining_node);
        }
    }
    pqueue_free(frontier, NULL);
}

List reconstruct_path(Dict came_from, Node start, Node goal, Graph graph, FILE *txt_file) {
    List path = new_list();
    Node current = goal;
    
    int step_count = 0;
    while (current != start) {
        Point current_point = getNodeInfo(graph, current);
        list_insert(path, current_point);

        if (txt_file != NULL) {
            double x, y;
            point_get_coordinates(current_point, &x, &y);
            
            fprintf(txt_file, "\tPasso %d: Siga para o ponto (%.2f, %.2f) - Nó %d\n", ++step_count, x, y, current);
        }
        
        Node *parent = (Node *) dict_get(came_from, current);
        if (parent == NULL) {
            if (txt_file != NULL) {
                fprintf(txt_file, "\nERRO: Caminho interrompido - não foi possível encontrar o nó pai.\n");
            }
            list_free(path, NULL);
            return new_list();
        }
        current = *parent;
    }
    
    Point start_point = getNodeInfo(graph, start);
    list_insert(path, start_point);
    
    return path;
}