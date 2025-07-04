#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "graph.h"

typedef struct {
    int vertex_amount;
    int max_vertex_amount;
} Graph_st;

typedef struct {
    Node from;
    Node to; 
    // char *
    double meanVel; 
    double length; 
} Edge_st; 

// typedef void *Edge;
// typedef void *Info;

static void alloc_error() {
    printf("ERROR: insuffienct memory for allocation");
    exit(1);
}

Graph createGraph(int nVert, bool directed) {
    if (nVert < 0) return NULL;

    Graph_st *graph = (Graph_st *) malloc(sizeof(Graph_st));
    if (graph == NULL) alloc_error();

    graph->vertex_amount = 0;
    graph->max_vertex_amount = nVert;
    return (Graph) graph;
}

Node addNode(Graph g, char *nome, Info info) {
    assert(g);
}
