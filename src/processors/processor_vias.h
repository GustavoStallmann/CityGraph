#ifndef PROCESSOR_VIAS_H
#define PROCESSOR_VIAS_H
#include "processor_dir.h"
#include "../data_structures/graph.h"
#include "../data_structures/smu_treap.h"

/*
 * @file: processor_vias.h 
 * @brief: process via files to create the city graph

 * @author: Gustavo Stallmann
*/

/*
 * Process the given via file
 * @param "Dir" the directory of via file
 * @param "SmuTreap" aux treap to insert the graph points
 * @return "Graph" the graph created from .via
*/
Graph via_process(Dir dir, SmuTreap aux_treap) ; 
#endif