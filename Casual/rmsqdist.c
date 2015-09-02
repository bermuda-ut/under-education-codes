/*=============================================================================
#     FileName: rmsqdist.c
#         Desc: calcaulates root mean squre distance 
#       Author: Max Lee (Ho Suk Lee)
#        Email: hoso1312@gmail.com
#     HomePage: http://imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2015-08-21 11:07:08
#      History:
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RMS_INVALID -1
#define RMS_VALID    0

int calc_rms(double X[], double Y[], int n, double *rms);

int
main(int argc, char **argv) {
    double X[1000], Y[1000], main_rms=0;
    int n=0;

    printf("Type X, Y: ");
    while(scanf("%lf %lf", &X[n], &Y[n]) == 2) {
        n++;
    }

    if (calc_rms(X,Y,n,&main_rms) == RMS_INVALID) {
        printf("Invalid input\n");
    }

    printf("main rms: %f\n", main_rms);

    return 0;
}

int 
calc_rms(double X[], double Y[], int n, double *rms) {
    if (n <= 0) {
        return RMS_INVALID;
    }
    int sum=0, i;

    for (i=0; i<n; i++) {
        sum += (X[i] - Y[i]) * (X[i] - Y[i]);
    }

    *rms = sqrt((0.0 + sum)/n);

    return RMS_VALID;
}
