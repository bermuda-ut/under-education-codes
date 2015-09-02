/*=============================================================================
#     FileName: printplus.c
#         Desc: program that prints plus symbol according to user input
#               Sample MST question for COMP10002
#       Author: Max Lee (Ho Suk Lee)
#        Email: hoso1312@gmail.com
#     HomePage: http://imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2015-08-20 01:36:23
#      History:
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>

#define MAX 10

void print_vert(int n);
void print_horz(int n);

int
main(int argc, char **argv) {
    int n;

    printf("Enter a value between 1 and %d: ", MAX);

    if (scanf("%d", &n) == 1 && n > 1 && n < MAX) {
        print_vert(n);
        print_horz(n);
        print_vert(n);
    } else {
        printf("Invalid input.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void
print_vert(int n) {
    int i, x;
    for (i=0; i<n-1; i++) {
        for (x=0; x<n-1; x++) {
            printf("  ");
        }
        printf("+\n");
    }
}

void
print_horz(int n) {
    int i;
    for (i=0; i<2*n-1; i++) {
        printf("+ ");
    }
    printf("\n");
}
