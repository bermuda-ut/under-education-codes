/*=============================================================================
#     FileName: toposort.c
#         Desc: funtions to sort graph topologically using DFS or Kahn
#               algorithm.
#       Author: Max Lee 719577
#        Email: hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-03-31 18:16:19
=============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "toposort.h"

bool *init_visited(int order);
void rec_dfs(Graph graph, int id, bool *visited, bool *tempMark, List *sortedList);
void disconnect_vertex(Graph *graph, int id, List *noDep);

/* Returns a list of topologically sorted vertices using the DFS method */
List dfs_sort(Graph graph) {
    bool    *visited    = init_visited(graph->order),
            *tempMark   = init_visited(graph->order);
    int     i;
    List    sortedList  = NULL;

    // gurantee visit every node
    for(i = 0; i < graph->order; i++) {
        rec_dfs(graph, i, visited, tempMark, &sortedList);
    }

    free(visited);
    free(tempMark);

    return sortedList;
}

/* Recurse visit vertices */
void rec_dfs(Graph graph, int id, bool *visited, bool* tempMark, List *sortedList) {
    if(tempMark[id] == true) {
        // cyclicity occurs when you come back to the initial starting node before
        // exiting it.
        fprintf(stderr, "Cycle detected. Not DAG, unable to sort.\n");
        exit(EXIT_FAILURE);
    }

    if(!visited[id]) {
        tempMark[id] = true;

        Vertex popped; // popped to visit
        while((popped = pop(&graph->vertices[id].out)) != NULL) {
            rec_dfs(graph, popped->id, visited, tempMark, sortedList);
        }

        int *data    = malloc(sizeof(int));
        *data        = id;

        visited[id]  = true;
        tempMark[id] = false;
        *sortedList  = push(*sortedList, data);
    }
}

/* Disconnects the vertex completely but leaves the vertex */
void disconnect_vertex(Graph *graph, int id, List *noDep) {
    Vertex toRemove = (*graph)->vertices+id;

    while(toRemove->out != NULL) {
        Vertex vertDist = pop(&(toRemove->out)); //vertex to disconnect
        ((*graph)->size)--;

        //remove inbound
        del(ptr_eq, toRemove, &(vertDist->in));

        // if the disconnection created an independent vertex
        if(noDep && vertDist->in == NULL) {
            *noDep = push(*noDep, vertDist);
        }
    }
}

/* Returns a list of topologically sorted vertices using the Kahn method */
List kahn_sort(Graph graph) {
    List    noDep = NULL,
            sortedList = NULL;
    int     i;

    // initial list of independent vertex
    for(i = 0; i < graph->order; i++) {
        if(graph->vertices[i].in == NULL)
            noDep = push(noDep, graph->vertices+i);
    }

    while(noDep) {
        Vertex current = (Vertex) pop(&noDep);

        int *data = malloc(sizeof(int));
        *data = current->id;

        sortedList = push(sortedList, data);

        disconnect_vertex(&graph, current->id, &noDep);
    }

    // if failed to remove all edges, it means cyclic
    if(graph->size > 0) {
        fprintf(stderr, "Cycle detected. Not DAG, unable to sort.\n");
        exit(EXIT_FAILURE);
    }

    sortedList = reverse(sortedList);

    return sortedList;
}

/* Uses graph to verify vertices are topologically sorted
 * by visiting the vertices in the list's order */
bool verify(Graph graph, List vertices) {
    bool*   visited = init_visited(graph->order);
    List    currVert = vertices;

    while(currVert) {
        int id = *(int*) currVert->data;
        // mark as visited
        visited[((Vertex)(currVert->data))->id] = true;

        // go through the in-s. If there are links between current
        // vertex and unvisitedv vertex, it is a proof that the list
        // is not sorted correctly.
        if(graph->vertices[id].in) {
            List currList = graph->vertices[id].in;
            int currId;

            while(currList) {
                currId = *(int*) currList->data;

                // illegal move detected
                if(visited[currId] == false)
                    return false;

                currList = currList->next;
            }
        }

        currVert = currVert->next;
    }
    return true;
}

/* return list of visited */
bool *init_visited(int order) {
    bool    *visited = malloc(order * sizeof(bool));
    int     i;

    // set all to false
    for(i = 0; i < order; i++)
        visited[i] = false;

    return visited;
}

