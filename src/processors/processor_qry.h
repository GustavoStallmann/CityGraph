#ifndef PROCESSOR_QRY_H
#define PROCESSOR_QRY_H

#include "processor_dir.h"
#include "../data_structures/graph.h"
#include "../data_structures/hash_table.h"
#include "../data_structures/smu_treap.h"

/* 
 * Process the query file and perform their operations
 * @param "qry" the qry file directory
 * @param "txt" the log txt file directory
 * @param "registers" the registers to save values
 * @param "Graph" the graph structure to perform the operations
 * @param "SmuTreap" auxiliar treap to help vector finding
 * @param "blocks_table" city blocks table
*/
void qry_process(Dir qry, Dir txt, Hash registers, Graph graph, SmuTreap aux_treap, Hash blocks_table);

#endif