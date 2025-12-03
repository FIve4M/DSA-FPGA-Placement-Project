#ifndef MOVE2D_H
#define MOVE2D_H

#include "grid.h"

// Returns random integer in [min, max]
int randInt2D(int min, int max);

// Randomly choose two distinct modules (ids in [0, moduleCount-1])
// and output them as m1, m2. The grid is *not* modified here.
void generateRandomModulePair(int moduleCount, int* m1, int* m2);

// Apply a 2D swap between two modules using grid + mod_x/mod_y
void applySwapMove2D(Grid* g, int mod_x[], int mod_y[], int m1, int m2);

// Randomly move a module to a random *empty* cell.
// Returns 1 on success, 0 if no empty cell exists.
// Does NOT modify grid/mod_x/mod_y; only outputs chosen module & target cell.
// The SA loop can then decide to accept/reject and call moveModuleTo().
int generateRandomMoveToEmptyCell(Grid* g,
                                  int moduleCount,
                                  int* module,
                                  int* target_r,
                                  int* target_c);

// Optional: helper – count empty cells
int countEmptyCells(Grid* g);

#endif
