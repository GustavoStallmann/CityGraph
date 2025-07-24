#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "dictionary.h"
#include "list.h"
#include "hash_table.h"

typedef struct Subgraph_st {
    char *name;
    List vertices;  
    List edges;     
    bool induced;   
} Subgraph_st;

typedef struct Graph_st {
    char *name; 
    bool directed; 
    int vertex_amount;
    int max_vertex_amount;
    Dict vertexes_dict; 
    Hash subgraphs;     
} Graph_st;

typedef struct Edge_st {
    Node from;
    Node to; 
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

    graph->subgraphs = new_hash(nVert);
    if (graph->subgraphs == NULL) {
        free(graph->name);
        free(graph);
        alloc_error();
    }

    graph->directed = directed; 
    graph->name = alloc_str(nomeGrafo);
    graph->max_vertex_amount = nVert; 
    graph->vertex_amount = 0;

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

    if (dict_key >= graph->max_vertex_amount) {
        return -1;
    }

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
    assert(nd >= 0);
    
    Graph_st *graph = (Graph_st *) g; 
    Node_st *node = (Node_st *) dict_get(graph->vertexes_dict, (int) nd); 
    if (node == NULL) return; 

    node->info = info; 
}

Edge addEdge(Graph g, Node from, Node to, Info info) {
    assert(g); 
    assert(from >= 0);
    assert(to >= 0); 

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
    
    // For undirected graphs, add the reverse edge
    if (!graph->directed) {
        Edge_st *reverse_edge = (Edge_st *) malloc(sizeof(Edge_st)); 
        if (reverse_edge == NULL) alloc_error(); 

        reverse_edge->from = to; 
        reverse_edge->to = from; 
        reverse_edge->info = info; 

        list_insert_end(to_node->adjacencies, reverse_edge); 
    }
    
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

typedef struct RemoveEdgeData_st {
    Node from_node;
    Node to_node;
    Info edge_info;
    List target_adjacencies;
    Edge_st *edge_to_remove;
} RemoveEdgeData_st;

static void find_reverse_edge(ListValue value, void *data) {
    Edge_st *edge = (Edge_st *) value;
    RemoveEdgeData_st *remove_data = (RemoveEdgeData_st *) data;
    
    if (edge != NULL && 
        edge->from == remove_data->to_node && 
        edge->to == remove_data->from_node &&
        edge->info == remove_data->edge_info) {
        remove_data->edge_to_remove = edge;
    }
}

void removeEdge(Graph g, Edge e) {
    assert(g); 
    assert(e);

    Graph_st *graph = (Graph_st *) g; 
    Edge_st *edge = (Edge_st *) e; 

    Node from_node = edge->from; 
    Node to_node = edge->to;
    Node_st *from_node_st = (Node_st *) dict_get(graph->vertexes_dict, (int) from_node);
    Node_st *to_node_st = (Node_st *) dict_get(graph->vertexes_dict, (int) to_node);
    
    list_remove_value(from_node_st->adjacencies, e, &list_compare_edge);   
    
    if (!graph->directed && to_node_st != NULL) {
        RemoveEdgeData_st remove_data = {
            .from_node = from_node,
            .to_node = to_node,
            .edge_info = edge->info,
            .target_adjacencies = to_node_st->adjacencies,
            .edge_to_remove = NULL
        };
        
        list_foreach(to_node_st->adjacencies, &find_reverse_edge, &remove_data);
        
        if (remove_data.edge_to_remove != NULL) {
            list_remove_value(to_node_st->adjacencies, remove_data.edge_to_remove, &list_compare_edge);
            free(remove_data.edge_to_remove);
        }
    }
    
    free(e);  // Free the edge after removing it from adjacency lists
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
    return list_search(from_node->adjacencies, &to, &list_compare_adjacency);
}

void adjacentEdges(Graph g, Node node, List arestasAdjacentes) {
    assert(g);
    assert(arestasAdjacentes);

    Graph_st *graph = (Graph_st *) g; 
    
    if (node < 0 || node >= dict_get_size(graph->vertexes_dict) || dict_is_empty(graph->vertexes_dict, node)) {
        return;
    }
    
    Node_st *from_node = (Node_st *) dict_get(graph->vertexes_dict, (int) node); 
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
        if (node == NULL || node->name == NULL) continue; 

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

/*
 *************
 * SUBGRAPH
 *************
*/

static Subgraph_st* create_subgraph(char *nomeSubgrafo) {
    assert(nomeSubgrafo);

    Subgraph_st *subgraph = (Subgraph_st *) malloc(sizeof(Subgraph_st));
    if (subgraph == NULL) alloc_error();

    subgraph->name = alloc_str(nomeSubgrafo);
    subgraph->vertices = new_list();
    subgraph->edges = new_list();
    subgraph->induced = false;

    if (subgraph->vertices == NULL || subgraph->edges == NULL) {
        if (subgraph->vertices) list_free(subgraph->vertices, free);
        if (subgraph->edges) list_free(subgraph->edges, NULL);
        free(subgraph->name);
        free(subgraph);
        alloc_error();
    }

    return subgraph;
}

void createSubgraphDG(Graph g, char *nomeSubgrafo, char *nomesVerts[], int nVert, bool comArestas) {
    assert(g);
    assert(nomeSubgrafo);
    assert(nomesVerts);
    assert(nVert >= 0);

    Graph_st *graph = (Graph_st *) g;
    
    if (hash_get(graph->subgraphs, nomeSubgrafo) != NULL) {
        return; 
    }

    Subgraph_st *subgraph = create_subgraph(nomeSubgrafo);
    subgraph->induced = comArestas;

    for (int i = 0; i < nVert; i++) {
        Node node = getNode(g, nomesVerts[i]);
        if (node != -1) {
            char *vertex_name = alloc_str(nomesVerts[i]);
            list_insert_end(subgraph->vertices, vertex_name);
        }
    }

    if (comArestas) {
        for (int i = 0; i < nVert; i++) {
            Node from_node = getNode(g, nomesVerts[i]);
            if (from_node == -1) continue;

            for (int j = 0; j < nVert; j++) {
                if (i == j) continue;
                
                Node to_node = getNode(g, nomesVerts[j]);
                if (to_node == -1) continue;

                Edge edge = getEdge(g, from_node, to_node);
                if (edge != NULL) {
                    list_insert_end(subgraph->edges, edge);
                }
            }
        }
    }

    hash_insert(graph->subgraphs, nomeSubgrafo, subgraph);
}

static bool find_edge_in_subgraph(ListValue value, void *target) {
    return value == target;
}

static bool find_vertex_by_name(ListValue value, void *target) {
    char *vertex_name = (char *) value;
    char *target_name = (char *) target;
    return strcmp(vertex_name, target_name) == 0;
}

typedef struct {
    Graph graph;
    Node from_node;
    List result_list;
} EdgeSearchData;

static void find_edges_from_node(void *value, callback_data call_data) {
    Edge edge = (Edge) value;
    EdgeSearchData *data = (EdgeSearchData *) call_data;
    
    Node from_node = getFromNode(data->graph, edge);
    if (from_node == data->from_node) {
        list_insert_end(data->result_list, edge);
    }
}

typedef struct {
    Graph graph;
    List result_list;
} NodeCollectionData;

static void collect_nodes(void *value, callback_data call_data) {
    char *vertex_name = (char *) value;
    NodeCollectionData *data = (NodeCollectionData *) call_data;
    
    Node node = getNode(data->graph, vertex_name);
    if (node != -1) {
        Node *node_ptr = malloc(sizeof(Node));
        if (node_ptr != NULL) {
            *node_ptr = node;
            list_insert_end(data->result_list, node_ptr);
        }
    }
}

Edge includeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e) {
    assert(g);
    assert(nomeSubgrafo);
    assert(e);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return NULL;
    }

    Edge existing_edge = (Edge) list_search(subgraph->edges, e, &find_edge_in_subgraph);
    if (existing_edge != NULL) {
        return e;
    }

    list_insert_end(subgraph->edges, e);
    return e;
}

bool existsEdgeSDG(Graph g, char *nomeSubgrafo, Edge e) {
    assert(g);
    assert(nomeSubgrafo);
    assert(e);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return false;
    }

    return list_search(subgraph->edges, e, &find_edge_in_subgraph) != NULL;
}

void excludeEdgeSDG(Graph g, char *nomeSubgrafo, Edge e) {
    assert(g);
    assert(nomeSubgrafo);
    assert(e);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return; 
    }

    list_remove_value(subgraph->edges, e, &find_edge_in_subgraph);
}

void adjacentEdgesSDG(Graph g, char *nomeSubgrafo, Node node, List arestasAdjacentes) {
    assert(g);
    assert(nomeSubgrafo);
    assert(arestasAdjacentes);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return; 
    }


    char *node_name = getNodeName(g, node);
    if (node_name == NULL) return;

    bool node_in_subgraph = list_search(subgraph->vertices, node_name, &find_vertex_by_name) != NULL;
    if (!node_in_subgraph) return;

    EdgeSearchData search_data = {g, node, arestasAdjacentes};
    list_foreach(subgraph->edges, &find_edges_from_node, &search_data);
}

void getAllNodesSDG(Graph g, char *nomeSubgrafo, List lstNodes) {
    assert(g);
    assert(nomeSubgrafo);
    assert(lstNodes);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return; 
    }

    NodeCollectionData collection_data = {g, lstNodes};
    list_foreach(subgraph->vertices, &collect_nodes, &collection_data);
}

void getAllEdgesSDG(Graph g, char *nomeSubgrafo, List lstEdges) {
    assert(g);
    assert(nomeSubgrafo);
    assert(lstEdges);

    Graph_st *graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return;
    }

    list_copy(subgraph->edges, lstEdges);
}

typedef struct {
    Graph source_graph;
    Graph target_graph;
} GraphCopyData;

static void copy_vertex_to_graph(void *value, callback_data call_data) {
    char *vertex_name = (char *) value;
    GraphCopyData *data = (GraphCopyData *) call_data;
    
    Node original_node = getNode(data->source_graph, vertex_name);
    if (original_node != -1) {
        Info node_info = getNodeInfo(data->source_graph, original_node);
        addNode(data->target_graph, vertex_name, node_info);
    }
}

static void copy_edge_to_graph(void *value, callback_data call_data) {
    Edge edge = (Edge) value;
    GraphCopyData *data = (GraphCopyData *) call_data;
    
    Node from_node = getFromNode(data->source_graph, edge);
    Node to_node = getToNode(data->source_graph, edge);
    Info edge_info = getEdgeInfo(data->source_graph, edge);
    
    char *from_name = getNodeName(data->source_graph, from_node);
    char *to_name = getNodeName(data->source_graph, to_node);
    
    if (from_name && to_name) {
        Node new_from = getNode(data->target_graph, from_name);
        Node new_to = getNode(data->target_graph, to_name);
        
        if (new_from != -1 && new_to != -1) {
            addEdge(data->target_graph, new_from, new_to, edge_info);
        }
    }
}

Graph produceGraph(Graph g, char *nomeSubgrafo) {
    assert(g);
    assert(nomeSubgrafo);

    Graph_st *original_graph = (Graph_st *) g;
    Subgraph_st *subgraph = (Subgraph_st *) hash_get(original_graph->subgraphs, nomeSubgrafo);
    
    if (subgraph == NULL) {
        return NULL;
    }

    Graph new_graph = createGraph(list_get_size(subgraph->vertices), original_graph->directed, subgraph->name);
    if (new_graph == NULL) return NULL;

    GraphCopyData copy_data = {g, new_graph};
    list_foreach(subgraph->vertices, &copy_vertex_to_graph, &copy_data);

    list_foreach(subgraph->edges, &copy_edge_to_graph, &copy_data);

    return new_graph;
}

void killDG(Graph g) {
    if (g == NULL) return;

    Graph_st *graph = (Graph_st *) g;
    
    for (int i = 0; i < dict_get_size(graph->vertexes_dict); i++) {
        if (dict_is_empty(graph->vertexes_dict, i)) continue;

        Node_st *node = (Node_st *) dict_get(graph->vertexes_dict, i);
        if (node != NULL) {
            if (node->name) free(node->name);
            if (node->adjacencies) list_free(node->adjacencies, NULL);
            free(node);
        }
    }
    
    if (graph->vertexes_dict) dict_free(graph->vertexes_dict);
    if (graph->subgraphs) hash_free(graph->subgraphs);
    if (graph->name) free(graph->name);
    free(graph);
}