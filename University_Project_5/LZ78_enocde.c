/*=============================================================================
#     FileName: LZ78_encode.c
#         Desc: My program for assignment 2, sir.
#               Sad that this is the final assignment for us from you, sir.
#       Author: Max Lee (Ho Suk Lee)
#        Email: hoso1312@gmail.com
#     HomePage: http://imnotbermuda.com
#      Version: 7.1.0
#   LastChange: 2015-10-15 20:10:40
===============================================================================
#
#                              Performance Summary
#
#                        File Name  | Size(kB) | Time
#                       ------------+----------+-------
#                        pg2600.txt |   3216   | 0.257s
#                        pg4000.txt |   8088   | 0.752s
#                       2pg4000.txt |   16172  | 1.626s
#                       4pg4000.txt |   32340  | 3.455s
#                                                   *measured without freeing.
# O(n).
# Precisely, nt+0.025*n^2 where n = newfilesize/oldfilesize, t = oldtime
# nt is dominant over 0.025*n^2 in almost all practical situations.
# Note : Including freeing increases the constant factor for n^2 slightly,
#        but nt is still more dominant
#
# Uses about 8MB RAM per MB file size
# Theoretically, hashing for node child will improve overall memory usage.
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

typedef int key_t;
/* int instead of char to accomodate special or non-English characters */

typedef struct node node_t;
struct node {
    /* Index of the child = the character
     * However, to reduce memort usage, variable width has been implemented.
     *  ie. to find 'a', node[a-min] where min=smallest child
     *
     * This results some situations save a lot of memory compared to having
     * fixed width.
     *  Ex) to store g, i, j
     *          node.min = g, node.max = j
     *          child[0] = g, child[1] = NULL, child[2] = i, child[3] = j
     *      to search i,
     *          node.child[i-node.min] => returns i */
    key_t data, max, min;
    int idx;
    node_t **child;
};

typedef struct {
    node_t *root;
} trie_t;

void free_trie(trie_t *trie);
void rec_free_node(node_t *node);
void insert_into_node(node_t *node, key_t key, int idx);
trie_t *gen_trie(void);
node_t *gen_node(void);
node_t *search_node(node_t *node, key_t key);

int
main(int argc, char **argv) {
    trie_t *dict = gen_trie();
    node_t *current_node, *last_known_node;
    int inserts = 1, i, last_known_idx, last_known_i, sec_known_idx, total;

    /* Initialization */
    last_known_idx = last_known_i = sec_known_idx = total = 0;
    current_node = last_known_node = dict->root;

    /* Instead of saving the entire file into an array, read the file and
     * process it as you read.
     * Trie is memory hungry enough, best to save memory where possible */
    while((i=getchar()) != EOF) {
        total++;
#if DEBUG
        if(i!=10) fprintf(stderr, "i = %d // char = %c\n", i, i);
        else fprintf(stderr, "i = 10 // char = </n>\n");
#endif

        if((current_node = search_node(current_node, i)) == NULL) {
            /* No match found.
             * Since at this point, its guranteed that previous values are
             * matching, simply add new character to the latest node */
#if DEBUG
            fprintf(stderr, " > NOT FOUND\n");
#endif
            insert_into_node(last_known_node, i, inserts++);

            printf("%c%d\n", i, last_known_idx);

            /* re-initialize */
            last_known_idx = 0;
            current_node = last_known_node = dict->root;
        } else {
            /* Keep track of history - affects constant for n^2 
             * Affects program speed slightly.
             * To negate this, reading the input in chunks then recording
             * history only for the last chunk will negate unneccesary
             * recording of history in previous chunks but this is
             * too minor impact to performance compared to amount of
             * complexity of the code */
            sec_known_idx   = last_known_idx;
            last_known_idx  = current_node->idx;
            last_known_node = current_node;
#if DEBUG
            fprintf(stderr, " > FOUND under index %d\n", current_node->idx);
#endif
        }

        last_known_i = i;

#if DEBUG
            fprintf(stderr, "----------\n");
#endif
    }

    if(last_known_idx != 0) {
        /* Handling exception situations where the last character was still
         * included in the count. This means if text is:
         *   (prev strings) . . X Y \0
         * ..XY were found in previously and present in dictionary.
         * Therefore following the formatting for the assignment, print
         * index for ..X then character Y */
        printf("%c%d\n", last_known_i, sec_known_idx); 
        inserts++;
    }

    /* Free trie. May not seem necessary since the program terminates
     * immediately after this stage however in case of practical use of
     * stacked calls, there it is :)
     * NOTE: In my Linux system, free did not actually reduce memory usage.
     *       This was true for this program and Alistair's malloc programs. */
    free_trie(dict);

    fprintf(stderr, "encode: %6d bytes input\n",total);
    fprintf(stderr, "encode: %6d factors generated\n",inserts-1);

    return 0;
}

node_t 
*gen_node(void) {
    /* Creates new node, set default values and returns pointer to it */
    node_t *new = (node_t*) malloc(sizeof(node_t));

    /* Initialization */
    new->idx   = new->min = 0;
    new->max   = -1;
    new->data  = '\0';
    new->child = NULL;

    return new;
}

trie_t 
*gen_trie(void) {
    /* Creates new trie, set default values and returns pointer to it */
    trie_t *trie = (trie_t*) malloc(sizeof(trie_t));
    trie->root   = gen_node(); /* dummy node */

    return trie;
}

node_t
*search_node(node_t *node, key_t key) {
    /* Returns poiter to the searching node,
     * If not found, return NULL */
#if DEBUG
    fprintf(stderr, " > Searching %c under %c\n", key, node->data);
    fprintf(stderr, " >  max %d\n >  min %d\n >  key %d\n",
            node->max, node->min, key);
#endif

    if (node->child != NULL && key <= node->max && key >= node->min) {
        /* Outside range = guranteed not exsisting
         * Note that when children have been generated,
         * empty no childnode = NULL therefore no guard needed to cover that*/
        return node->child[(int)key-node->min];
    }

    return NULL;
}

void
insert_into_node(node_t *node, key_t key, int idx) {
    /* Insert child into node.
     * If there is pre-exsisting child, function will overwrite and delete
     * data silently. Losing data > losing track of data + memory leak
     *
     * Will adjust size of child accordingly using realloc
     *
     * Note that overall in this program, no overwriting occurs. Overwriting
     * fucntionality was coded for the completeness of the function. */
    int i, size, diff;
    node_t *new_node = gen_node();
    size = node->max - node->min + 1; /* size = number of children*/

    new_node->data = key;
    new_node->idx  = idx;

#if DEBUG
    if(node->child != NULL) {
        fprintf(stderr, "MEMORY - INITIAL.\n");
        for(i=0; i<size; i++) {
            fprintf(stderr, "%p ", node->child[i]);
        }
        fprintf(stderr, "\n");
    }
#endif

    if(size == 0) {
        /* First insertion ;)
         * size == 0 will work since each node has been intialized as
         * max=-1, min=0 */
        node->child    = malloc(sizeof(node_t*));
        node->child[0] = NULL; /* Cleanse for testing phase */

        node->max = node->min = key;

    } else if(node->max < key) {
        /* Need space on the right for new child */
        diff = key - node->max; /* diff = number of slots to be added */

        node->child = realloc(node->child, (size+diff) * sizeof(node_t*));

        for(i=size; i < size+diff; i++) {
            /* Fill new slots with NULL */
            node->child[i] = NULL;
        }

        node->max = key;

    } else if(node->min > key) {
        /* Need space on the left for new child */
        diff = node->min - key; /* diff = number of slots to be added */

        node->child = realloc(node->child, (size+diff) * sizeof(node_t*));

        for(i=size-1; i>=0; i--) {
            /* Shift the array to the right to accomodate new min, starting
             * from the right */
            node->child[i+diff] = node->child[i];
        }

        for(i=0; i < diff; i++) {
            /* Fill new slots with NULL */
            node->child[i] = NULL;
        }

        node->min = key;
    }

    if(node->child[(int)key-node->min] != NULL) {
        /* If there is data, remove it. Losing data is better than losing 
         * track of data and leaking memory. This is here for the
         * completeness of the fumction rather than program's requirement. */
#if DEBUG
        fprintf(stderr, "[WARNING] Overwritten data. ");
        fprintf(stderr, "Address : %p\n", node->child[(int)key-node->min]);
#endif
        rec_free_node(node->child[(int)key-node->min]);
    }

    node->child[(int)key-node->min] = new_node;
#if DEBUG
    fprintf(stderr, "Inserted %c under %c\n", new_node->data, node->data);
#endif
}

void 
free_trie(trie_t *trie) {
    /* Free entire trie and its nodes */
    rec_free_node(trie->root);
    free(trie);
}

void
rec_free_node(node_t *node) {
    /* Free node's child then itself */
    int i;

    if(node->child != NULL) {
        /* free children */
        for(i = 0; i < node->max-node->min+1; i++) {
            if(node->child[i] != NULL) {
                /* child is a node, free node */
                rec_free_node(node->child[i]);
            } else {
                /* child is NULL slot, free slot */
                free(node->child[i]);
            }
        }
    }

    free(node->child);
    free(node);
}

/*
#
# Algorithms are fun!
#  _   __                
# | | / /                
# | |/ /  ___  ___ _ __  
# |    \ / _ \/ _ \ '_ \
# | |\  \  __/  __/ |_) |
# \_| \_/\___|\___| .__/ 
#  _____ _        | |    
# |_   _| |       |_|
#   | | | |_
#   | | | __|
#  _| |_| |_ 
#  \___/ \__|
#  _____ _                 _      
# /  ___(_)               | |     
# \ `--. _ _ __ ___  _ __ | | ___ 
#  `--. \ | '_ ` _ \| '_ \| |/ _ \
# /\__/ / | | | | | | |_) | |  __/  _
# \____/|_|_| |_| |_| .__/|_|\___| ( )
#  _____ _          | |  _     _   |/
# /  ___| |         |_| (_)   | |
# \ `--.| |_ _   _ _ __  _  __| |
#  `--. \ __| | | | '_ \| |/ _` |
# /\__/ / |_| |_| | |_) | | (_| |
# \____/ \__|\__,_| .__/|_|\__,_|
#                 | |            
#                 |_|            
#
#                      - Lord Alistair
#
# ASCII art generated from patorjk.com/software/tagg
#
*/
