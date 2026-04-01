#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <string.h>

#include "init.h"
#include "utils.h"

// Global variables
int GRID_X, GRID_Y, NX, NY;
int NUM_Points, Maxiter;
double dx, dy;

int main()
{
    
    NX = 250;
    NY = 100;

    GRID_X = NX + 1;
    GRID_Y = NY + 1;

    dx = 1.0 / NX;
    dy = 1.0 / NY;

    NUM_Points = 14000000;
    Maxiter = 10;

    int thread_list[5] = {2, 4, 6, 8, 16};

    printf("Threads\tCase\tInterpTime\tMoverTime\tTotal\n");

   
    // LOOP OVER THREADS

    for(int t = 0; t < 5; t++)
    {
        int threads = thread_list[t];
        omp_set_num_threads(threads);

        printf("\nRunning for %d threads...\n", threads);

        double *mesh_value =
            (double *)calloc(GRID_X * GRID_Y, sizeof(double));

        Points *points =
            (Points *)calloc(NUM_Points, sizeof(Points));

        if(points == NULL || mesh_value == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }

        // =========================
        // A5 (WITH INSERTION)
        // =========================

        initializepoints(points);

        double total_interp_A5 = 0.0;
        double total_mover_A5 = 0.0;

        for(int iter = 0; iter < Maxiter; iter++)
        {
            memset(mesh_value, 0, GRID_X * GRID_Y * sizeof(double));

            clock_t start_interp = clock();
            interpolation(mesh_value, points);
            clock_t end_interp = clock();

            clock_t start_move = clock();
            mover_immediate_parallel(points, dx, dy);
            clock_t end_move = clock();

            total_interp_A5 += (double)(end_interp - start_interp) / CLOCKS_PER_SEC;
            total_mover_A5 += (double)(end_move - start_move) / CLOCKS_PER_SEC;
        }

        double total_A5 = total_interp_A5 + total_mover_A5;

        printf("%d\tA5\t%lf\t%lf\t%lf\n",
               threads,
               total_interp_A5,
               total_mover_A5,
               total_A5);

        // =========================
        // A4 (WITHOUT INSERTION)
        // =========================

        initializepoints(points);

        double total_interp_A4 = 0.0;
        double total_mover_A4 = 0.0;

        for(int iter = 0; iter < Maxiter; iter++)
        {
            memset(mesh_value, 0, GRID_X * GRID_Y * sizeof(double));

            clock_t start_interp = clock();
            interpolation(mesh_value, points);
            clock_t end_interp = clock();

            clock_t start_move = clock();
            mover_immediate_parallel(points, dx, dy);
            clock_t end_move = clock();

            total_interp_A4 += (double)(end_interp - start_interp) / CLOCKS_PER_SEC;
            total_mover_A4 += (double)(end_move - start_move) / CLOCKS_PER_SEC;
        }

        double total_A4 = total_interp_A4 + total_mover_A4;

        printf("%d\tA4\t%lf\t%lf\t%lf\n",
               threads,
               total_interp_A4,
               total_mover_A4,
               total_A4);

        free(mesh_value);
        free(points);
    }

    printf("\nDone!\n");

    return 0;
}
