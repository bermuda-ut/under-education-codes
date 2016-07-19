/*=============================================================================
#     FileName: graphio.c
#         Desc: This module provides all the IO functionality related to graphs
#       Author: Clement Poh (cpoh@unimelb.edu.au)
#               Max Lee 719577
#        Email: hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-04-08 02:53:26
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "graphio.h"

#define MAX_LINE_LEN 256
#define INDENT       "  "
#define EDGE_SEP     ' '

void add_vertex(Graph graph, char *label);

/* Loads the graph from input */
Graph load_graph(char *input) {
    FILE    *inputFile = fopen(input, "r");
    char    currLine[MAX_LINE_LEN];
    int     size,
            numCurr = 0, // currently reading line number
            v1, v2,
            sepIdx;
    Graph   newGraph;

    if(inputFile == NULL) {
        fprintf(stderr, "Unable to open the file.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(currLine, MAX_LINE_LEN, inputFile) != NULL) {
        if(numCurr == 0) {
            // get size of the graph
            size     = atoi(currLine);
            newGraph = new_graph(size);

        } else if(numCurr <= size) {
            // Read vertex labels
            add_vertex(newGraph, currLine);

        } else {
            // Read edges
            //
            // NOTE: more elegant solution would be using
            // fscanf(inputFile, "%d %d\n", &v1, &v2)
            // but I embrace assignment's challenge/suggestion of
            // using fgets, and I figured out C-ish solution :P

            // Find index of the separater,
            sepIdx = 0;
            while(currLine[sepIdx] != EDGE_SEP)
                sepIdx++;
            currLine[sepIdx] = '\0'; // Art of C

            // then convert to two integers, using pointers before
            // and after separater. WILL work due to two '\0's.
            v1 = atoi(currLine),
            v2 = atoi(currLine+sepIdx+1);

            add_edge(newGraph, v1, v2);
        }

        numCurr++;
    }

    fclose(inputFile);

    return newGraph;
}

/* Add vertex to the graph */
void add_vertex(Graph graph, char *label) {
    // get rid of new line. This will NOT cause memory leak - label here
    // was not malloced.
    if(label[strlen(label) - 1] == '\n')
        label[strlen(label) - 1] = '\0';

    // strlen here stops at first '\0' (if \n was eliminated)
    char* newLabel = malloc(sizeof(char) * (strlen(label) + 1));

    // malloc check
    if(!newLabel) {
        fprintf(stderr, "Failed to allocate memory for label.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newLabel, label); // will stop at first '\0'. Art.

    // initilize values
    graph->vertices[graph->order].id    = graph->order;
    graph->vertices[graph->order].label = newLabel;
    graph->vertices[graph->order].in    = NULL;
    graph->vertices[graph->order].out   = NULL;
    // ^ initial NULL to make initial adding edge easier

    (graph->order)++;
}

/* Prints the graph in DOT format */
void print_graph(char *output, Graph graph) {
    FILE    *fpOut = fopen(output, "w");
    int     i;
    Vertex  currVertex;

    fprintf(fpOut, "digraph {\n");

    for(i = 0; i < graph->order; i++) {
        fprintf(fpOut, INDENT); // required indentation to match examples

        // print parent label printing
        currVertex = (graph->vertices)+i;
        fprintf(fpOut, "%s", currVertex->label);

        // print parent's children
        if((*currVertex).out != NULL) {
            fprintf(fpOut, " -> {");
            print_list(print_vertex_label, fpOut, (*currVertex).out);
            fprintf(fpOut, "}");
        }

        fprintf(fpOut, "\n");
    }

    fprintf(fpOut, "}\n");

    fclose(fpOut);
}

/* Prints the destination vertex label surrounded by spaces */
void print_vertex_label(FILE *file, void *vertex) {
    if (vertex)
        fprintf(file, " %s ", ((Vertex)vertex)->label);
}

// Kills me I cannot make code below pretty since I didn't write them

/* Prints the id of a vertex then a newline */
void print_vertex_id(FILE *file, void *vertex) {
    if (vertex)
        fprintf(file, "%d\n", ((Vertex)vertex)->id);
}

/* Returns a sequence of vertices read from file
 * Was tempted to beautify code format for this one but didn't =( */
List load_vertex_sequence(FILE *file, Graph graph) {
    const char *err_duplicate = "Error: duplicate vertex %d %s\n";
    const char *err_order = "Error: graph order %d, loaded %d vertices\n";
    List list = NULL;
    int id;

    while(fscanf(file, "%d\n", &id) == 1) {
        assert(id >= 0);
        assert(id < graph->order);

        if (!insert_if(id_eq, graph->vertices + id, &list)) {
            fprintf(stderr, err_duplicate, id, graph->vertices[id].label);
            exit(EXIT_FAILURE);
        }
    }

    if (len(list) != graph->order) {
        fprintf(stderr, err_order, graph->order, len(list));
        exit(EXIT_FAILURE);
    }

    return list;
}

