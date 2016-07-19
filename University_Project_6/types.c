/*=============================================================================
#     FileName: types.c
#         Desc: assignment 2
#       Author: Clement Poh
#               Max Lee
#        Email: cpoh@unimelb.edu.au
#               hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-05-17 17:16:41
=============================================================================*/
#include "types.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Returns whether n1 is equal to n2 */
bool int_eq(long e1, long e2) {
    return e1 == e2;
}

/* Prints n to file */
void int_print(FILE *file, long e) {
    fprintf(file, " %ld", e);
}

/* Returns whether str1 is equal to str2 */
bool str_eq(char *str1, char *str2) {
    return !strcmp(str1, str2);
}

/* Returns a copy of src */
char *str_copy(char *src) {
    int len = strlen(src);
    char *cpy = malloc(sizeof(char) * len);
    src[len-1] = '\0'; // requirement according to formum post by Clement

    strcpy(cpy, src);
    return cpy;
}

/* Prints str to file */
void str_print(FILE *file, char *str) {
    fprintf(file, " %s", str);
}

