/*=============================================================================
#     FileName: search-text.c
#         Desc: Program for searching query in text and ranking output.
#       Author: Max Lee
#        Email: hoso1312@gmail.com
#     HomePage: http://imnotbermuda.com
#      Version: 6.3.4
#   LastChange: 2015-09-17 23:42:05
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define QUERYMAX  100  /* Maximum query length, excluding final '\0' */
#define STRINGMAX 1000 /* Maximum line length, excluding final '\0' */
#define RANKCOUNT 10   /* Number of ranked output */
#define DEBUG     0    /* Toggle debug output */
#define DOUBLE_TOLERANCE 1e-10
/* Maximum difference between double to be considered equal */
#define BLANK_FILLER '0'
/* Single character to fill the non-significant f values in debug output.
 * Turn it into space ' ' for better readability in debug output.
 * Default set to '0' to match the formatting of the question */

/*===========================================================================*/

void cut_query(const char *query, char *assignquery, int amt, char side);
void double_swap(double *a, double *b);
void int_swap(int *a, int *b);
void str_swap(char *a, char *b);
void KMP_gen_F(char *query, int qlen, int *F);
void line_sort(int linenum[], double score[], char line[][STRINGMAX+1],
                int size);
void score_sort(double score[], int linenum[], char line[][STRINGMAX+1],
                int size);
int myline(char line[STRINGMAX+1], int limit);
int double_equal(double *a, double *b);
int KMP_search(char *query, char *str, int qlen, int strlen);
double calc_runs(char *query, char *line);
double calc_score(char *query, char *line);

/*===========================================================================*/

int
main(int argc, char **argv) {
    /* main is the body of the program where it calculates each step
     * requirements. Each steps are not in separate function since they are
     * called only once and they are not general action */
    char templine[STRINGMAX+1], rankline[RANKCOUNT][STRINGMAX+1];
    int linecnt=0, curr_entries=0, i, ranklineno[RANKCOUNT], result;
    double tempscore, rankscore[RANKCOUNT];
    /* Kindly note each rank has 3 components: string, score and linenumber
     * and each component has been assigned to seperate arrays.
     * Using struct would have been easier to code and read but we'd only
     * learnt it during the last week before deadline.
     * Blame our guru Alistair, the Great Mighty Lord :P */

    if(argc != 2) {
        /* Must specify query */
        printf("Invalid query.\nUsage: %s QUERY < file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((i=(int)strlen(argv[1])) > QUERYMAX) {
        /* Query greater than specified breaks the program therefore guard.
         * Assigned to i since it won't be used ever again */
        printf("The query is %d characters long. "
               "Maximum allowed is %d characters.\n", i, QUERYMAX);
        exit(EXIT_FAILURE);
    }

    printf("S0: query = %s \n", argv[1]);
    /* space after %s to match the format of the example output*/

    while((result = myline(templine, STRINGMAX)) != EOF) {
        /* read line, save the result of reading into result */
        linecnt++;
        if(result == 0) {
            /* If not empty line */
            printf("---\n");
            printf("%s\n", templine);

            printf("S1: line %5d, bytes = %2d\n", linecnt,
                    (int)strlen(templine));

            printf("S2: line %5d, score = %6.3f\n", linecnt,
                    calc_runs(argv[1], templine));

            tempscore = calc_score(argv[1], templine);
            printf("S4: line %5d, score = %6.3f\n", linecnt, tempscore);

            if(curr_entries < RANKCOUNT && tempscore > 0) {
                /* Simply fill in into rank list when under limit */
                rankscore[curr_entries] = tempscore;
                ranklineno[curr_entries] = linecnt;
                strcpy(rankline[curr_entries], templine);
                curr_entries++;
            } else {
                /* sort then compare with smallest value then decide
                 * whether to swap the data */
                score_sort(rankscore, ranklineno, rankline, curr_entries);
                line_sort(ranklineno, rankscore, rankline, curr_entries);

                if(rankscore[curr_entries-1] < tempscore) {
                    /* Linear sort sorts descending order therefore
                     * swap with the last value. No need to worry about
                     * line number since guranteed to be larger. */
                    rankscore[curr_entries-1] = tempscore;
                    ranklineno[curr_entries-1] = linecnt;
                    str_swap(rankline[curr_entries-1], templine);
                }
            }
        }
    }

    printf("------------------------------------------------\n");

    score_sort(rankscore, ranklineno, rankline, curr_entries);
    line_sort(ranklineno, rankscore, rankline, curr_entries);

    for(i=0; i<curr_entries; i++) {
        printf("S3: line %5d, score = %6.3f\n", ranklineno[i], rankscore[i]); 
        printf("%s\n", rankline[i]);
        printf("---\n");
    }

    return 0;
}

/*===========================================================================*/

int
myline(char line[STRINGMAX+1], int limit) {
    /* myline reads a line and store into line[] until specifed limit.
     * Because prompting the user that the line has exceeded the limit is not
     * a requirement and it does not break the program, the program continues
     * to run silently. */
    int i, j=0;

    if ((i=getchar()) == EOF) {
        return EOF;
    } else if(i != '\n') {
        /* Initialize */
        line[j++] = i;
    } else {
        /* when it is blank line */
        return 1;
    }

    while((i=getchar()) != '\n' && j<limit) {
        line[j++] = i;
    }
    line[j] = '\0'; /* Adding nullbyte independent from limit */

    return 0;
}

/*===========================================================================*/

double
calc_runs(char *query, char *line) {
    /* Calculates maximum match of the query in the line
     * Assumes valid string query and line
     *
     * Visual Represenation of the algorithm:
     *
     * 1.
     * str:     a b c d e a b c d e a b c d e
     * qur: a b c
     * Compare str(a) qur(c)
     *
     * 2.
     * str:   a b c d e a b c d e a b c d e
     * qur: a b c
     * Compare str(b) qur(c), str(a) qur(c)
     *
     *    ...
     *
     * End.
     * str: a b c d e a b c d e a b c d e
     * qur:                             a b c
     * Compare str(e) qur(a) */
    int i=0, j=0, k, qlen, llen;
    double currstreak=0, maxstreak=0;

    qlen = strlen(query);
    llen = strlen(line);

    i=0;
    while(i < qlen + llen - 1) {
        /* qlen + llen -1 : check the diagram above */
        currstreak = 0;
        k = i;
        for(j=qlen-1; j>=0; j--) {
            /* Start checking from behind, j=index checking */
            if(k>=0 && k<=llen) {
                /* This condition makes sure nothing outside line gets
                 * compared with the query */

                if(query[j] == line[k]) {
                    currstreak++;
                } else {
                    currstreak = 0;
                }

                if(currstreak > maxstreak) {
                    maxstreak = currstreak;
                }
            }
            k--;
        }

        if(maxstreak == qlen) {
            /* Maximum possible reached, no need to search further */
            return maxstreak;
        }

        i++;
    }

    return maxstreak;
}

/*===========================================================================*/

double
calc_score(char *query, char *line) {
    /* Calculate score for the line using the equation provided in the
     * question.
     * Potential Algorithmic Improvement: Reduce usage of strcpy */
    char clline[STRINGMAX+1], clquery[QUERYMAX+1], tempquery[QUERYMAX+1],
         tempquery2[QUERYMAX+1];
    int i, j, qlen, llen=strlen(line), tempcount=0;
    double score=0;

    /* Clean up query to small letters and alphanumeric.
     * Not a general thing to do therefore not in a seperate function */
    for(i=j=0; query[i] != '\0'; i++) {
        /* i sticks to the query and always increment
         * j sticks to the new query and increment only when 
         * query[i] is alphanumeric */
        if(isalnum(query[i])) {
            if(isupper(query[i])) {
                clquery[j] = tolower(query[i]);
            } else {
                clquery[j] = query[i];
            }
            j++;
        }
    }
    clquery[j] = '\0';
    qlen = strlen(clquery);

    /* Clean up line to lower cases.
     * Not a general thing to do therefore not in a seperate function */
    for(i=0; line[i] != '\0'; i++) {
        if(isupper(line[i])) {
            clline[i] = tolower(line[i]);
        } else {
            clline[i] = line[i];
        }
    }
    clline[i] = '\0';

#if DEBUG
        printf("DB: ");
        for(i=0; clquery[i] != '\0'; i++) {
            /* Print out header of the matrix table */
            printf("%3c", clquery[i]);
        }
        printf("\n");

        for(i=0; i<qlen; i++) {
            /* i = amount to cut from left */
            printf("DB: ");
            cut_query(clquery, tempquery, i, 'l');

            for(j=0; j<i; j++) {
                /* Initial fillers ie. will lower triangle in the end */
                printf("%3c", BLANK_FILLER);
            }

            for(j=qlen-i; j>0; j--) {
                /* j = amount to cut from right.
                 * cut_query does not follow industry standard of string
                 * slicing with string index.
                 * Please refer to cut_query documentation if this part is not
                 * self-explanatory */
                cut_query(tempquery, tempquery2, j, 'r');
                tempcount = KMP_search(tempquery2, clline, qlen-i-j+1, llen);
                /* Currently processing query length = qlen - i - (j-1)
                 * j-1 because the query was cut by (j-1), NOT j.
                 * Not using strlen(tempquery2) for slight efficiency.
                 * (Kindly note how cut_query works)*/
                if(tempcount == 0) {
                    break;
                    /* For efficiency, break if 0 appears since it is
                     * guranteed to be 0 until the end. Then simply fill with
                     * indicated filler. */
                }

                printf("%3d", tempcount);
            }

            for(; j>0; j--) {
                /* Filler after it is guranteed that no further matches */
                printf("%3c", BLANK_FILLER);
            }
            printf("\n");
        }
#endif

    /* Score calculation based on the equation given.
     * Kindly note that magic numbers 3 and 2 in the for loop conditions are
     * part of the equation. */
    for(i=0; i<=qlen-3; i++) {
        cut_query(clquery, tempquery, i, 'l');
        for(j=i+2; j<=qlen-1; j++) {
            /* Refer to cut_query documentation if this part is not
             * self-explanatory */
            cut_query(tempquery, tempquery2, qlen-j, 'r');
            tempcount = KMP_search(tempquery2, clline, j-i+1, llen);
            /* Currently processing query length = qlen - i - (qlen-(j+1))
             *                                   = j - i + 1
             * Not using strlen(tempquery2) for slight efficiency.*/

            if(tempcount > 0) {
                /* Calculations using log base 2 according to the equation
                 * NOT using log2() since it is defined only in C99 and
                 * just to be sure my program compiles and works everywhere */
                score += (double) (j-i-1) * (j-i-1) * 
                    log(2+tempcount) / log(2);
            }
        }
    }

    return score = score / (log(30+llen) / log(2));
    /* Magic number 30 is part of the equation */
}

/*===========================================================================*/

void
cut_query(const char *query, char *assignquery, int amt, char side) {
    /* Cut query like how Python slices. Cut amout = amt
     * ie.      amt = the starting index when cut from left.
     *     qlen-amt = the final included index relative when cut from right.
     * Side can only be 'r'/'l', respectively represents right/left
     * The manipulated query is assigned to assignedquery
     *
     * > WARNING <
     * This function is potentially confusing for practical use for anyone
     * other than the author because it does not use the industry-stanard
     * of string slicing with index but uses amount and side to slice.
     * Honestly, this was my mistake for not thinking it through carefully
     * before starting to code.
     * For non-acadmeic use, this function require revision to index based
     * string slicing. */
    char tempquery[QUERYMAX+1];
    int i, j=0;
    int qlen = (int) strlen(query);

    if(side == 'l') {
        /* cut left hand side */
        for(i=amt; i<qlen; i++) {
            /* Pretty standard.
             * Python equivalent :
             * string[amt:] */
            tempquery[j] = query[i];
            j++;
        }
    } else if(side == 'r') {
        /* cut right left side */
        for(i=0; i<qlen-amt+1; i++) {
            /* Potentially extremely confusing.
             * Python equivalent :
             * string[:strlen-amt+1] */
            tempquery[j] = query[i];
            j++;
        }
    } else {
        printf("Unrecoverable error. Failed to cut query.\n");
        exit(EXIT_FAILURE);
    }
    tempquery[j] = '\0';
    strcpy(assignquery, tempquery);
}

/*===========================================================================*/

void
score_sort(double score[], int linenum[], char line[][STRINGMAX+1], int size) {
    /* Linearly sort array in descending order according to score.
     * Linear sort based on COMP10002 lectures by Alistair Moffat.
     * There is no advantage of replacing this algorithm with quicksort */
    int i, j;
    /* i = currently evaluating element
     * j = iterate previous values and move the current element */
    for(i=0; i<size; i++) {
        for(j=i; j>0 && !double_equal(score+j, score+j-1) &&
                score[j] > score[j-1]; j--) {
            /* Swap every components */
            double_swap(score+j, score+j-1);
            int_swap(linenum+j, linenum+j-1);
            str_swap(line[j], line[j-1]);
        }
    }
}

/*===========================================================================*/

void
line_sort(int linenum[], double score[], char line[][STRINGMAX+1], int size) {
    /* Linearly sort array in ascending order according to lines for rank
     * entities with equal score.
     * Linear sort based on COMP10002 lectures by Alistair Moffat.
     * Score is priority therefore swapping only occurs for equal scores */
    int i, j;
    /* i = currently evaluating element
     * j = iterate previous values and move the current element */
    for(i=0; i<size; i++) {
        for(j=i; j>0 && double_equal(score+j, score+j-1) &&
                linenum[j] < linenum[j-1]; j--) {
            /* Swap every components */
            double_swap(score+j, score+j-1);
            int_swap(linenum+j, linenum+j-1);
            str_swap(line[j], line[j-1]);
        }
    }
}

/*===========================================================================*/

void
KMP_gen_F(char *query, int qlen, int *F) {
    /* Generate KMP failure array and copy it into F.
     * Based on pseudo code in COMP10002 lecture slides by Alistair Moffat */
    int posQ=2, cnd=0;
    /* Initialization */
    F[0] = -1;
    F[1] = 0;

    while(posQ < qlen) {
        if(query[posQ-1] == query[cnd]) {
            cnd++;
            F[posQ++] = cnd;
        } else if(cnd > 0) {
            cnd = F[cnd];
        } else {
            F[posQ++] = 0;
        }
    }
}

/*===========================================================================*/

int
KMP_search(char *query, char *str, int qlen, int strlen) {
    /* Based on the pseudo code on Wikipedia KMP Algorithm page
     * https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
     * Adjusted to search multiple occurences. */
    int posL=0, posQ=0, freq=0, F[qlen];
    /* posL : Current position in the line
     * posQ : Current position in the query */

    KMP_gen_F(query, qlen, F); /* Generate failure array */

    while(posL <= strlen-qlen) {
        if(str[posL+posQ] == query[posQ]) {
            posQ++;
            if(posQ == qlen) {
                /* The equal values were the last to compare */
                freq++;
                posL = posL + posQ-1 - F[posQ-1];
                /* Follow the same methodology as failure state.
                 * -1 after posQ since was incremented previously. */
                posQ = 0;
            }
        } else {
            posL = posL + posQ - F[posQ];
            posQ = 0;
        }
    }

    return freq;
}

/*===========================================================================*/

int
double_equal(double *a, double *b) {
    /* Determines wheter two double values are equal within error */
    if(fabs(*a-*b) <= DOUBLE_TOLERANCE) {
        return 1;
    }
    return 0;
};

/*===========================================================================*/

void
double_swap(double *a, double *b) {
    /* swap double values */
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/*===========================================================================*/

void
int_swap(int *a, int *b) {
    /* swap integer values */
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/*===========================================================================*/

void
str_swap(char *a, char *b) {
    /* swap string values */
    char temp[STRINGMAX+1];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

/*
#
#                               Algorithms are fun!
#______   ___                              __    __ 
/\  _  \ /\_ \                          __/\ \__/\ \
\ \ \L\ \\//\ \      __     ___   _ __ /\_\ \ ,_\ \ \___     ___ ___     ____
#\ \  __ \ \ \ \   /'_ `\  / __`\/\`'__\/\ \ \ \/\ \  _ `\ /' __` __`\  /',__\
# \ \ \/\ \ \_\ \_/\ \L\ \/\ \L\ \ \ \/ \ \ \ \ \_\ \ \ \ \/\ \/\ \/\ \/\__, `\
#  \ \_\ \_\/\____\ \____ \ \____/\ \_\  \ \_\ \__\\ \_\ \_\ \_\ \_\ \_\/\____/
#   \/_/\/_/\/____/\/___L\ \/___/  \/_/   \/_/\/__/ \/_/\/_/\/_/\/_/\/_/\/___/
#                    /\____/ 
#                    \_/__/
#                                __     _ __    __   
#                              /'__`\  /\`'__\/'__`\
#                             /\ \L\.\_\ \ \//\  __/
#                             \ \__/.\_\\ \_\\ \____\
#                              \/__/\/_/ \/_/ \/____/
#
#                              ___                  __     
#                             /'___\               /\ \
#                            /\ \__/  __  __    ___\ \ \
#                            \ \ ,__\/\ \/\ \ /' _ `\ \_\
#                             \ \ \_/\ \ \_\ \/\ \/\ \/_/_
#                              \ \_\  \ \____/\ \_\ \_\/\_\
#                               \/_/   \/___/  \/_/\/_/\/_/
#
# ASCII art generated from patorjk.com/software/taag
#
*/

