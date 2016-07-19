/*=============================================================================
#     FileName: graph.c
#         Desc: Graph generation funtions
#       Author: Max Lee 719577
#        Email: hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-03-31 18:18:09
=============================================================================*/
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

/* Returns a pointer to a new graph with order vertices */
Graph new_graph(int order) {
    Graph graphNew      = malloc(sizeof(*graphNew));

    graphNew->order     = graphNew->size = 0;
    graphNew->vertices  = malloc(sizeof(*graphNew->vertices) * order);

    return graphNew;
}

/* Returns whether aim and vertex are pointing to the same location */
bool ptr_eq(void *aim, void *vertex) {
    return (aim == vertex);
}

/* Returns whether aim and vertex have the same id */
bool id_eq(void *aim, void *vertex) {
    return (((Vertex) aim) -> id == ((Vertex) vertex) -> id);
}

/* Add the edge from v1 to v2 to graph */
void add_edge(Graph graph, int v1, int v2) {
    graph->vertices[v1].out = push(graph->vertices[v1].out, (graph->vertices) + v2);
    graph->vertices[v2].in  = push(graph->vertices[v2].in,  (graph->vertices) + v1);
    (graph->size)++;
}

/* Free the memory allocated to graph */
void free_graph(Graph graph) {
    Vertex  current;
    int     i;

    for(i = 0; i < graph->order; i++) {
        current = (graph->vertices) + i;

        // free edges
        free_list(current->in);
        free_list(current->out);

        free(current->label);
    }

    free(graph->vertices);
    free(graph);
}

/*
    41 6C 67 6F 72 69 74 68 6D 73  61 72 65  66 75 6E
     _    _                  _ _   _
    / \  | | __ _  ___  _ __(_) |_| |__  _ __ ___  ___
   / _ \ | |/ _` |/ _ \| '__| | __| '_ \| '_ ` _ \/ __|
  / ___ \| | (_| | (_) | |  | | |_| | | | | | | | \__ \
 /_/   \_\_|\__, |\___/|_|  |_|\__|_| |_|_| |_| |_|___/
            |___/
                     __ _ _ __ ___
                    / _` | '__/ _ \
                   | (_| | | |  __/
                    \__,_|_|  \___|
                 _____ _   _ _   _   _
                |  ___| | | | \ | | | |
                | |_  | | | |  \| | | |
                |  _| | |_| | |\  | |_|
                |_|    \___/|_| \_| (_)

      41 6C 69 73 74 61 69 72  4D 6F 66 66 61 74
*/
