#ifndef PROCESSOR_QRY_H
#define PROCESSOR_QRY_H

#include "processor_dir.h"
#include "../data_structures/graph.h"
#include "../data_structures/hash_table.h"

/* 
 * Process the query file and perform their operations
 * @param "qry" the qry file directory
 * @param "txt" the log txt file directory
 * @param "registers" the registers to save values
 * @param "Graph" the graph structure to perform the operations
*/
void qry_process(Dir qry, Dir txt, Hash registers, Graph graph);

#endif