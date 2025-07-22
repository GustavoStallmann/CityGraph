#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "dictionary.h"
#include "list.h"

typedef struct Graph_st {
    char *name; 
    bool directed; 
    int vertex_amount;
    int max_vertex_amount;
    Dict vertexes_dict; 
} Graph_st;

typedef struct Edge_st {
    Node from;
    Node to; 
    bool active; 
    Info info; 
} Edge_st; 

typedef struct Node_st {
    char *name; 
    List adjacencies; 
    Info info; 
} Node_st; 

static void alloc_error() {
    printf("ERROR: insufficient memory for allocation\n");
    exit(1);
}

static char* alloc_str(char *str) {
    assert(str); 

    int str_len = strlen(str); 
    char *new_str = malloc(str_len * sizeof(char) + 1); 
    if (new_str == NULL) alloc_error();

    strcpy(new_str, str); 
    return new_str;
}

Graph createGraph(int nVert, bool directed, char *nomeGrafo) {
    if (nVert < 0) return NULL;

    Graph_st *graph = (Graph_st *) malloc(sizeof(Graph_st));
    if (graph == NULL) alloc_error();

    graph->vertexes_dict = new_dict(nVert); 
    if (graph->vertexes_dict == NULL) {
        free(graph); 
        alloc_error();
    }

    graph->directed = directed; 
    graph->name = alloc_str(nomeGrafo);
    graph->max_vertex_amount = nVert; 

    return (Graph) graph;
}

int getMaxNodes(Graph g) {
    assert(g); 
    Graph_st *graph = (Graph_st *) g; 
    return graph->max_vertex_amount;
}

int getTotalNodes(Graph g) {
    assert(g); 
    Graph_st *graph = (Graph_st *) g; 
    return graph->vertex_amount;
}

Node addNode(Graph g, char *nome, Info info) {
    assert(g);
    assert(nome); 
    assert(info); 

    Graph_st *graph = (Graph_st *) g; 

    int dict_key;
    for (
        dict_key = 0; 
        dict_key < graph->max_vertex_amount && !dict_is_empty(graph->vertexes_dict, dict_key); 
        dict_key++
    );

    Node_st *node = (Node_st *) malloc(sizeof(Node_st)); 
    if (node == NULL) alloc_error(); 

    node->adjacencies = new_list(); 
    if (node->adjacencies == NULL) {
        free(node); 
        alloc_error(); 
    }

    node->name = alloc_str(nome);
    if (node->name == NULL) {
        free(node); 
        alloc_error(); 
    }

    node->info = info; 
    dict_put(graph->vertexes_dict, dict_key, node); 

    graph->vertex_amount++; 
    return dict_key; 
}

Node getNode(Graph g, char *nome) {
    assert(g); 

    Graph_st *graph = (Graph_st *) g; 
    for (int i = 0; i < dict_get_size(graph->vertexes_dict); i++) {
        if (dict_is_empty(graph->vertexes_dict, i)) continue; 

        Node_st *node = dict_get(graph->vertexes_dict, i); 
        if (node == NULL) continue; 

        if (strcmp(node->name, nome) != 0) continue; 
        return i; 
    }

    return -1; 
}

Info getNodeInfo(Graph g, Node node) {
    assert(g); 

    Graph_st *graph = (Graph_st *) g; 
    Node_st *found_node = (Node_st *) dict_get(graph->vertexes_dict, (int) node);
    if (found_node == NULL) return NULL; 

    return found_node->info; 
}

char* getNodeName(Graph g, Node node) {
    assert(g); 

    Graph_st *graph = (Graph_st *) g; 
    Node_st *found_node = (Node_st *) dict_get(graph->vertexes_dict, (int) node);
    if (found_node == NULL) return NULL; 
    
    return found_node->name; 
}

void setNodeInfo(Graph g, Node nd, Info info) {
    assert(g);
    assert(nd);
    
    Graph_st *graph = (Graph_st *) g; 
    Node_st *node = (Node_st *) dict_get(graph->vertexes_dict, (int) nd); 
    if (node == NULL) return; 

    node->info = info; 
}

Edge addEdge(Graph g, Node from, Node to, Info info) {
    assert(g); 
    assert(from);
    assert(to); 

    Graph_st *graph = (Graph_st *) g; 
    Node_st *from_node = (Node_st *) dict_get(graph->vertexes_dict, (int) from); 
    Node_st *to_node = (Node_st *) dict_get(graph->vertexes_dict, (int) to); 
    if (from_node == NULL || to_node == NULL) return NULL; 
    
    Edge_st *new_edge = (Edge_st *) malloc(sizeof(Edge_st)); 
    if (new_edge == NULL) alloc_error(); 

    new_edge->from = from; 
    new_edge->to = to; 
    new_edge->info = info; 

    list_insert_end(from_node->adjacencies, new_edge); 
    return new_edge;     
}

static bool find_edge(ListValue value, void *target) {
    Edge_st *edge = (Edge_st *) value; 
    Node *search_node = (Node *) target; 

    return edge->to == *search_node;
}

Edge getEdge(Graph g, Node from, Node to) {
    assert(g); 

    Graph_st *graph = (Graph_st *) g; 
    Node_st *from_node = (Node_st *) dict_get(graph->vertexes_dict, (int) from); 
    Node_st *to_node = (Node_st *) dict_get(graph->vertexes_dict, (int) to); 
    if (from_node == NULL || to_node == NULL) return NULL; 

    return (Edge) list_search(from_node->adjacencies, &to, &find_edge); 
}

Node getFromNode(Graph g, Edge e) {
    (void) g;
    assert(g); 
    assert(e);

    Edge_st *edge = (Edge_st *) e; 

    return edge->from; 
}

Node getToNode(Graph g, Edge e) {
    (void) g;
    assert(g); 
    assert(e);

    Edge_st *edge = (Edge_st *) e; 

    return edge->to; 
}

Info getEdgeInfo(Graph g, Edge e) { 
    (void) g;
    assert(g); 
    assert(e);

    Edge_st *edge = (Edge_st *) e; 

    return edge->info; 
}

void setEdgeInfo(Graph g, Edge e, Info info) {
    (void) g;
    assert(g); 
    assert(e);

    Edge_st *edge = (Edge_st *) e; 

    edge->info = info; 
}

static bool list_compare_edge(ListValue value, void *target) {
    Edge_st *node_edge = (Edge_st *) value; 
    Edge_st *target_edge = (Edge_st *) target; 

    return node_edge == target_edge; 
}

void removeEdge(Graph g, Edge e) {
    assert(g); 
    assert(e);

    Graph_st *graph = (Graph_st *) g; 
    Edge_st *edge = (Edge_st *) e; 

    Node from_node = edge->from; 
    Node_st *found_node = (Node_st *) dict_get(graph->vertexes_dict, (int) from_node);
    list_remove_value(found_node->adjacencies, e, &list_compare_edge);   
}

static bool list_compare_adjacency(ListValue value, void *target) {
    Edge_st *edge = (Edge_st *) value; 
    Node *to_node = (Node *) target; 
    return edge->to == *to_node; 
}

bool isAdjacent(Graph g, Node from, Node to) {
    assert(g); 

    Graph_st *graph = (Graph_st *) g; 
    Node_st *from_node = (Node_st *) dict_get(graph->vertexes_dict, (int) from);
    Node_st *to_node = (Node_st *) dict_get(graph->vertexes_dict, (int) to);

    if (from_node == NULL || to_node == NULL) return false;
    return list_search(from_node->adjacencies, to_node, list_compare_adjacency);
}

void adjacentEdges(Graph g, Node node, List arestasAdjacentes) {
    assert(g);
    assert(node);
    assert(arestasAdjacentes);

    Graph_st *graph = (Graph_st *) g; 
    Node_st *from_node = (Node_st *) dict_get(graph, (int) node); 
    if (from_node == NULL || from_node->adjacencies == NULL) return; 

    list_copy(from_node->adjacencies, arestasAdjacentes);
}

void getNodeNames(Graph g, List nomesNodes) {
    assert(g);
    assert(nomesNodes);

    Graph_st *graph = (Graph_st *) g; 
    for (int i = 0; i < dict_get_size(graph->vertexes_dict); i++) {
        if (dict_is_empty(graph->vertexes_dict, i)) continue;

        Node_st *node = (Node_st *) dict_get(graph->vertexes_dict, i); 
        if (node == NULL || node->name) continue; 

        char *copy_name = alloc_str(node->name);
        if (copy_name == NULL) continue; 

        list_insert_end(nomesNodes, copy_name);
    }
}

void getEdges(Graph g, List arestas) {
    assert(g); 
    assert(arestas); 

    Graph_st *graph = (Graph_st *) g; 
    for (int i = 0; i < dict_get_size(graph->vertexes_dict); i++) {
        if (dict_is_empty(graph->vertexes_dict, i)) continue;

        Node_st *node = (Node_st *) dict_get(graph->vertexes_dict, i); 
        if (node == NULL || node->adjacencies == NULL) continue; 

        list_copy(node->adjacencies, arestas);
    }
}