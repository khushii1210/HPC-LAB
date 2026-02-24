#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Serial interpolation 
void interpolation(double *mesh_value, Points *points) {
        memset(mesh_value, 0, GRID_X * GRID_Y * sizeof(double));

    for (int p = 0; p < NUM_Points; p++) {

        double x = points[p].x;
        double y = points[p].y;

        int i = (int)(x / dx);
        int j = (int)(y / dy);

        if (i >= NX) i = NX - 1;
        if (j >= NY) j = NY - 1;

        double Xi = i * dx;
        double Yj = j * dy;

        double lx = x - Xi;
        double ly = y - Yj;

        double w00 = (dx - lx) * (dy - ly);
        double w10 = lx * (dy - ly);
        double w01 = (dx - lx) * ly;
        double w11 = lx * ly;

        double fi = 1.0;

        mesh_value[j * GRID_X + i]         += w00 * fi; // (i,j)
        mesh_value[j * GRID_X + (i + 1)]   += w10 * fi; // (i+1,j)
        mesh_value[(j + 1) * GRID_X + i]   += w01 * fi; // (i,j+1)
        mesh_value[(j + 1) * GRID_X + (i + 1)] += w11 * fi; // (i+1,j+1)
    }
}

// Write mesh to file
void save_mesh(double *mesh_value) {

    FILE *fd = fopen("Mesh.out", "w");
    if (!fd) {
        printf("Error creating Mesh.out\n");
        exit(1);
    }

    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            fprintf(fd, "%lf ", mesh_value[i * GRID_X + j]);
        }
        fprintf(fd, "\n");
    }

    fclose(fd);
}