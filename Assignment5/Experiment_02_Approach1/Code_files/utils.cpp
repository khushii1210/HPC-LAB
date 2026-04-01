#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

// Access global variables defined in main.cpp
extern int GRID_X, GRID_Y;
extern int NUM_Points;
extern double dx, dy;


/*
   Bilinear interpolation
   Each particle deposits weight 1.0 to surrounding 4 grid points
*/

void interpolation(double *mesh_value, Points *points)
{
    for(int p = 0; p < NUM_Points; p++)
    {
        double xp = points[p].x;
        double yp = points[p].y;

        int i = (int)(xp / dx);
        int j = (int)(yp / dy);

        // Clamp to avoid boundary overflow
        if(i >= GRID_X - 1) i = GRID_X - 2;
        if(j >= GRID_Y - 1) j = GRID_Y - 2;

        double x0 = i * dx;
        double y0 = j * dy;

        double wx = (xp - x0) / dx;
        double wy = (yp - y0) / dy;

        double w00 = (1 - wx) * (1 - wy);
        double w10 = wx * (1 - wy);
        double w01 = (1 - wx) * wy;
        double w11 = wx * wy;

        int idx00 = j * GRID_X + i;
        int idx10 = j * GRID_X + (i + 1);
        int idx01 = (j + 1) * GRID_X + i;
        int idx11 = (j + 1) * GRID_X + (i + 1);

        // Deposit particle weight = 1.0
        mesh_value[idx00] += w00;
        mesh_value[idx10] += w10;
        mesh_value[idx01] += w01;
        mesh_value[idx11] += w11;
    }
}


/*
   Serial mover
*/

void mover_serial(Points *points, double deltaX, double deltaY)
{
    for(int p = 0; p < NUM_Points; p++)
    {
        double rx = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double ry = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        double newx = points[p].x + rx * deltaX;
        double newy = points[p].y + ry * deltaY;


        if(newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
        {
            points[p].x = (double)rand() / RAND_MAX;
            points[p].y = (double)rand() / RAND_MAX;
        }
        else
        {
            points[p].x = newx;
            points[p].y = newy;
        }
    }
}


/*
   Parallel mover
*/

void mover_deferred(Points *points, double deltaX, double deltaY)
{
    int deleted = 0;

    for(int p = 0; p < NUM_Points; p++)
    {
        double rx = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double ry = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        double newx = points[p].x + rx * deltaX;
        double newy = points[p].y + ry * deltaY;

        if(newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
        {
            points[p].x = -1.0;  // mark void
            points[p].y = -1.0;
            deleted++;
        }
        else
        {
            points[p].x = newx;
            points[p].y = newy;
        }
    }

    // Insert new particles
    for(int p = 0; p < NUM_Points; p++)
    {
        if(points[p].x < 0.0)
        {
            points[p].x = (double)rand() / RAND_MAX;
            points[p].y = (double)rand() / RAND_MAX;
        }
    }
}

void mover_deferred_parallel(Points *points, double deltaX, double deltaY)
{
#pragma omp parallel for
    for(int p = 0; p < NUM_Points; p++)
    {
        unsigned int seed = 1234 + p;

        double rx = (rand_r(&seed) / (double)RAND_MAX) * 2.0 - 1.0;
        double ry = (rand_r(&seed) / (double)RAND_MAX) * 2.0 - 1.0;

        double newx = points[p].x + rx * deltaX;
        double newy = points[p].y + ry * deltaY;

        if(newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
        {
            points[p].x = -1.0;
            points[p].y = -1.0;
        }
        else
        {
            points[p].x = newx;
            points[p].y = newy;
        }
    }

#pragma omp parallel for
    for(int p = 0; p < NUM_Points; p++)
    {
        unsigned int seed = 5678 + p;

        if(points[p].x < 0.0)
        {
            points[p].x = rand_r(&seed) / (double)RAND_MAX;
            points[p].y = rand_r(&seed) / (double)RAND_MAX;
        }
    }
}


void mover_parallel(Points *points, double deltaX, double deltaY)
{
#pragma omp parallel for
    for(int p = 0; p < NUM_Points; p++)
    {
        unsigned int seed = 1234 + p;

        double rx = (rand_r(&seed) / (double)RAND_MAX) * 2.0 - 1.0;
        double ry = (rand_r(&seed) / (double)RAND_MAX) * 2.0 - 1.0;

        double newx = points[p].x + rx * deltaX;
        double newy = points[p].y + ry * deltaY;

        if(newx < 0.0 || newx > 1.0 || newy < 0.0 || newy > 1.0)
        {
            points[p].x = rand_r(&seed) / (double)RAND_MAX;
            points[p].y = rand_r(&seed) / (double)RAND_MAX;
        }
        else
        {
            points[p].x = newx;
            points[p].y = newy;
        }
    }
}


/*
   Save mesh
*/

void save_mesh(double *mesh_value)
{
    FILE *fd = fopen("Mesh.out", "w");
    if (!fd)
    {
        printf("Error creating Mesh.out\n");
        exit(1);
    }

    for (int j = 0; j < GRID_Y; j++)
    {
        for (int i = 0; i < GRID_X; i++)
        {
            fprintf(fd, "%lf ", mesh_value[j * GRID_X + i]);
        }
        fprintf(fd, "\n");
    }

    fclose(fd);
}
