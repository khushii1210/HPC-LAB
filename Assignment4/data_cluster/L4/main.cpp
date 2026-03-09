#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "init.h"
#include "utils.h"

// Global variables
int GRID_X, GRID_Y, NX, NY;
int NUM_Points, Maxiter;
double dx, dy;

int main() {

    // Fixed parameters for Experiment 03
    NX = 1000;
    NY = 400;
    NUM_Points = 14000000;   // 14 million
    Maxiter = 10;

    GRID_X = NX + 1;
    GRID_Y = NY + 1;
    dx = 1.0 / NX;
    dy = 1.0 / NY;

    omp_set_num_threads(4);

    double *mesh_value =
        (double *) calloc(GRID_X * GRID_Y, sizeof(double));

    Points *points =
        (Points *) calloc(NUM_Points, sizeof(Points));

    if(points == NULL || mesh_value == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize ONCE
    initializepoints(points);

    printf("Iter\tInterp\t\tMover\t\tTotal\n");

    for(int iter = 0; iter < Maxiter; iter++) {

        clock_t start_interp = clock();
        interpolation(mesh_value, points);
        clock_t end_interp = clock();

        clock_t start_move = clock();
        mover_parallel(points, dx, dy);   
        clock_t end_move = clock();

        double interp_time =
            (double)(end_interp - start_interp) / CLOCKS_PER_SEC;

        double mover_time =
            (double)(end_move - start_move) / CLOCKS_PER_SEC;

        double total = interp_time + mover_time;

        printf("%d\t%lf\t%lf\t%lf\n",
               iter+1, interp_time, mover_time, total);
    }

    free(mesh_value);
    free(points);

    return 0;
}
