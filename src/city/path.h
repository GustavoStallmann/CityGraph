#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include "../data_structures/graph.h"
#include "../data_structures/dictionary.h"
#include "../data_structures/list.h"
#include "point.h"

/*
 * Perform A* pathfinding algorithm from start to goal
 * @param "graph" the graph instance
 * @param "start" the starting node
 * @param "goal" the goal node
 * @param "use_length" if true, use street length for distance; if false, use mean velocity
 */
void a_star(Graph graph, Node start, Node goal, Dict *came_from, Dict *cost_so_far, bool use_length);

/*
 * Reconstruct the path from start to goal
 * @param "came_from" dictionary containing parent relationships from A*
 * @param "start" the starting node
 * @param "goal" the goal node
 * @param "graph" the graph instance to get node info from
 * @return "List" containing the path from start to goal (as Points)
 */
List reconstruct_path(Dict came_from, Node start, Node goal, Graph graph);

#endif
