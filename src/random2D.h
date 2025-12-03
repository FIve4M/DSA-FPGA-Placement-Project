#ifndef RANDOM2D_H
#define RANDOM2D_H

#include "grid.h"

// Randomly place all modules across the grid (no collisions)
// moduleCount must be <= rows*cols
void randomInitialPlacement2D(Grid* g, int moduleCount, int mod_x[], int mod_y[]);

#endif
