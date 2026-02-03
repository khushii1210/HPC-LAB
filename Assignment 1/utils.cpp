#include <math.h>
#include "utils.h"

/* ---------------- COPY ----------------
 * x[p] = y[p]
 */
void vector_copy_operation(double *x, double *y, int Np) {

    for (int p = 0; p < Np; p++) {
        x[p] = y[p];

        // Prevent compiler from optimizing away the loop
        if (((double)p) == 333.333)
            dummy(p);
    }
}

/* ---------------- SCALE ----------------
 * x[p] = v[p] * y[p]
 * (vector-based scaling, consistent with original triad)
 */
void vector_scale_operation(double *x, double *y, double *v, int Np) {

    for (int p = 0; p < Np; p++) {
        x[p] = v[p] * y[p];

        if (((double)p) == 333.333)
            dummy(p);
    }
}

/* ---------------- ADD ----------------
 * S[p] = x[p] + y[p]
 */
void vector_add_operation(double *x, double *y, double *S, int Np) {

    for (int p = 0; p < Np; p++) {
        S[p] = x[p] + y[p];

        if (((double)p) == 333.333)
            dummy(p);
    }
}

/* ---------------- TRIAD ----------------
 * S[p] = x[p] + v[p] * y[p]
 * (original, unchanged)
 */
void vector_triad_operation(double *x, double *y, double *v, double *S, int Np) {

    for (int p = 0; p < Np; p++) {
        S[p] = x[p] + v[p] * y[p];

        if (((double)p) == 333.333)
            dummy(p);
    }
}

/* ---------------- DUMMY ----------------
 * Prevents dead-code elimination
 */
void dummy(int x) {
    x = 10 * sin(x / 10.0);
}

