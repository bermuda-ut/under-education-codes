/*=============================================================================
#     FileName: extra.c
#         Desc: assignment 2
#       Author: Max Lee
#    StudentID: 719577
#        Email: hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-05-20 01:42:47
=============================================================================*/
#include "extra.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define PRINTPOINT    stdout // pointer to print random constants array
#define GET_TIME      false  // for big-O evalulation collide_dumb and collide_clever
#define I_DUNNO_STATS false  // to gurantee 100% no collision, 
                             // incase you are not a believer of statistics

#define MAX_TRY       100    // max tries for dumb collision for a given length
#define MAX_SIZE      255    // max size of the hashtable

#define INIT_SIZE     5      // starting(minimum) generated string size
#define START_ASCII   33     // starting ascii value for generated string
#define MAX_ASCII     126    // Ideal max ascii value of generated string
#define MAX_LEN       42     /* max len <= 256 of generated string
                                Lower this value, faster to generate
                                Increase this value, more unique strings can be generated
                                Ass2 requirement is generate 2 strings,
                                I have set this value way beyond safe levels. */

typedef struct {
    int a, b, c;
    // Could have size and array of ints + size and initialize the tuple with malloc.
    // But since this is only for the assignment so.. easy way out with fixed number
    // of variables :P
} tuple_t;

bool isPrime(unsigned int n) {
    unsigned int i;
    for(i = 2; i * i <= n; i++)
        if(n % i == 0)
            return false;
    return true;
}

unsigned int nextPrime(unsigned int n) {
    n++;
    while(!isPrime(n))
        n++;
    return n;
}

unsigned int gcd(unsigned int a, unsigned int b) {
    if(a < b) { // a must be the greater one
        unsigned int t = a;
        a = b;
        b = t;
    }

    if(b == 0) return a;

    return gcd(b, a % b);
}

tuple_t extEuclid(unsigned int a, unsigned int b) {
    if(a < b) { // a must be the greater one
        unsigned int t = a;
        a = b;
        b = t;
    }

    if(b == 0) return (tuple_t){1, 0, a};

    tuple_t t = extEuclid(b, a % b);
    return (tuple_t){t.b, t.a - (a/b) * t.b, t.c};
}

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {
    // fprintf(stderr, "Next prime is %d\n", nextPrime(2 * n));
    return nextPrime(2 * n);
}

int* getRandArray(int seed, int size) {
    int *arr, i;

    fprintf(PRINTPOINT, "%d\n", MAX_SIZE + 1); // new requirement

    arr = malloc(sizeof(*arr) * MAX_SIZE);

    srand(seed);
    for(i = 0; i < MAX_SIZE; i++) {
        arr[i] = rand() % size;
        fprintf(PRINTPOINT, "%d\n", arr[i]); // new requirement
    }

    return arr;
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed
 * although character can be anything <255, only 'pretty' characters are chosen
 * for the 'prettiness' of the output */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
    static bool   first    = true;
    int           *randArr = getRandArray(seed, size);
    unsigned int  randSize, i, try;
    unsigned char *line    = malloc(sizeof(*line) * MAX_LEN);

#if I_DUNNO_STATS
    unsigned char **used   = malloc(sizeof(*used) * n);
    unsigned int useCount  = 0;
    // index randsize will be '\0'. Therefore, randsize < MAXLEN.
#endif

    if(first == true) { // if virgin, initialize universal hash
        first = false;
        srand(seed);
        universal_hash(line, size);
    }

#if GET_TIME
    clock_t start = clock();
    fprintf(stderr, "N: %d\nSIZE: %d\n", n, size);
#endif

    while(n > 0) {
        try = 0;
        randSize = rand() % (MAX_LEN - INIT_SIZE - 1) + INIT_SIZE;
        // -1 for \0 slot

        while(try <= MAX_TRY) {
            for(i = 0; i < randSize; i++)
                line[i] = rand() % (MAX_ASCII - START_ASCII - 1) + START_ASCII;


            line[randSize] = '\0';

            if(universal_hash(line, size) == 0) {
#if I_DUNNO_STATS
                for(i = 0; i < useCount; i++) {
                    // make sure it is not duplicate
                    if(strcmp((char*) used[i], (char*) line) == 0)
                        break;
                }

                used[useCount] = malloc(sizeof(unsigned char) * (strlen((char*)line) + 1));
                strcpy((char*)used[useCount], (char*)line);
                // fprintf(stderr, "USED %d: %s\n", useCount, used[useCount]);
                useCount++;
#endif

                fprintf(stdout, "%s\n", line);
                n--;
                break; // now generate new random size!
            }
        }
    }

#if GET_TIME
    clock_t end = clock();
    fprintf(stderr, "TIME TAKEN: %.5f s\n", ((double) end - start) / CLOCKS_PER_SEC);
#endif

#if I_DUNNO_STATS
    for(i = 0; i < useCount; i++) {
        free(used[i]);
    }
    free(used);
#endif
    free(randArr);
    free(line);
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed
 * although character can be anything <255, only 'pretty' characters are chosen
 * for the 'prettiness' of the output */
void collide_clever(unsigned int size, unsigned int seed, int n) {
    static bool   first = true;
    int           randSize, sum, gen, i, x, toChange, *randArr = NULL;
    unsigned char y, *line = malloc(sizeof(*line) * MAX_LEN);
    // index randsize will be '\0'. Therefore, randsize < MAXLEN.

#if I_DUNNO_STATS
    unsigned char **used   = malloc(sizeof(*used) * n);
    int           useCount = 0;
#endif

    if(size > 255) {
        fprintf(stderr, "Size is too big. Outside assignment scope!\n");
        fprintf(stderr, "I had spent ages addressing that but after nerf,\n");
        fprintf(stderr, "and reached the state where generated string hash to one index for any size.\n");
        fprintf(stderr, "But no point working further on and submitting risky long code.\n");
        fprintf(stderr, "Anyway, terminating in sass :P\n");
        exit(EXIT_FAILURE);
    }

    // initialize universal hash's random array
    if(first == true) { // some say there are still first == true for most CompSc students
        first = false;
        srand(seed);
        universal_hash(line, size);
    }

    // find out which index to work on
    randArr = getRandArray(seed, size);
    i = 0;
    while(randArr[i] == 0)
        i++;
    toChange = i;

#if GET_TIME
    clock_t start = clock();
    fprintf(stderr, "N: %d\nSIZE: %d\n", n, size);
#endif

    // initialize my random array
    while(n > 0) {
        sum = 0;

        randSize       = rand() % (MAX_LEN - INIT_SIZE - 1) + INIT_SIZE;
        line[randSize] = '\0';

        for(i = 0; i < randSize; i++) {
            if(i != toChange) {
                line[i] = rand() % (MAX_ASCII - START_ASCII - 1) + START_ASCII;
                sum += (line[i] * randArr[i]);
            }
        }

        // explanation of how and why this works and the algorithm is in the report, question 7.
        gen       = size - (sum % size);
        tuple_t t = extEuclid(size, randArr[toChange]);

        x = t.b * gen / t.c;

        if(x < 0)
            y = size - ((-x) % size);
        else
            y = x % size;

        if(y < START_ASCII && size + y < MAX_ASCII) {
            y += size * (1 + (START_ASCII - y) / size);
            // minimum multiple of size to add to make y fall within given range
        } else if(y == '\n' || y == 127 || y <= 32) { // 127 = DEL ASCII
            // if the string genrated has newline character, solution is not valid since
            // strings are loaded with fgets().
            // this happens is about 0.3% of the time.
            // Forum post requirement: avoid characters from 0-32.
            continue;
            // had attempted to add minimum multiple of size,
            // faster to generate new rather than resolve
        }

        line[toChange] = y;

#if I_DUNNO_STATS
        for(i = 0; i < useCount; i++) {
            // make sure it is not duplicate
            if(strcmp((char*) used[i], (char*) line) == 0)
                break;
        }

        used[useCount] = malloc(sizeof(unsigned char) * (strlen((char*)line) + 1));
        strcpy((char*)used[useCount], (char*)line);
        // fprintf(stderr, "USED %d: %s\n", useCount, used[useCount]);
        useCount++;
#endif

        fprintf(stdout, "%s\n", line);
        n--;
    }

#if GET_TIME
    clock_t end = clock();
    fprintf(stderr, "TIME TAKEN: %.5f s\n", ((double) end - start) / CLOCKS_PER_SEC);
#endif

#if I_DUNNO_STATS
    for(i = 0; i < useCount; i++) {
        free(used[i]);
    }
    free(used);
#endif
    free(randArr);
    free(line);
}

