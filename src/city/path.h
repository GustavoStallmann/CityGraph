#ifndef PATH_H
#define PATH_H

#include "../data_structures/graph.h"
#include "../data_structures/dictionary.h"
#include "../data_structures/list.h"
#include "point.h"

/*
 * Perform A* pathfinding algorithm from start to goal
 * @param "graph" the graph instance
 * @param "start" the starting node
 * @param "goal" the goal node
 */
void a_star(Graph graph, Node start, Node goal);

/*
 * Reconstruct the path from start to goal
 * @param "came_from" dictionary containing parent relationships from A*
 * @param "start" the starting node
 * @param "goal" the goal node
 * @return "List" containing the path from start to goal
 */
List reconstruct_path(Dict came_from, Node start, Node goal);

#endif
