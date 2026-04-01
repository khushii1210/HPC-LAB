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

int main()
{
    // Grid configuration (change for each experiment)
    NX = 250;
    NY = 100;

    GRID_X = NX + 1;
    GRID_Y = NY + 1;

    dx = 1.0 / NX;
    dy = 1.0 / NY;

    Maxiter = 10;

    // Particle range for Experiment 1
    long particle_range[5] = {100, 10000, 1000000, 100000000, 1000000000};

    printf("Particles\tInterpTime\tMoverTime\tTotal\n");

    for(int caseid = 0; caseid < 5; caseid++)
    {
        NUM_Points = particle_range[caseid];

        double *mesh_value =
            (double *)calloc(GRID_X * GRID_Y, sizeof(double));

        Points *points =
            (Points *)calloc(NUM_Points, sizeof(Points));

        if(points == NULL || mesh_value == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }

        // Initialize particles ONCE
        initializepoints(points);

        double total_interp = 0.0;
        double total_mover = 0.0;

        for(int iter = 0; iter < Maxiter; iter++)
        {
            clock_t start_interp = clock();
            interpolation(mesh_value, points);
            clock_t end_interp = clock();

            clock_t start_move = clock();
            mover_immediate_parallel(points, dx, dy); // use serial or parallel here
            clock_t end_move = clock();

            double interp_time =
                (double)(end_interp - start_interp) / CLOCKS_PER_SEC;

            double mover_time =
                (double)(end_move - start_move) / CLOCKS_PER_SEC;

            total_interp += interp_time;
            total_mover += mover_time;
        }

        double total = total_interp + total_mover;

        printf("%ld\t%lf\t%lf\t%lf\n",
               particle_range[caseid],
               total_interp,
               total_mover,
               total);

        free(mesh_value);
        free(points);
    }

    return 0;
}
