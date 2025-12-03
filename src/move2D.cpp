#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "move2D.h"
#include "grid.h"

// ------------------------------------------------------
// Random integer in [min, max]
// ------------------------------------------------------
int randInt2D(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// ------------------------------------------------------
// Randomly choose two *distinct* module IDs in [0, moduleCount-1]
// ------------------------------------------------------
void generateRandomModulePair(int moduleCount, int* m1, int* m2) {
    if (moduleCount <= 1) {
        *m1 = 0;
        *m2 = 0;
        return;
    }

    int a = randInt2D(0, moduleCount - 1);
    int b = randInt2D(0, moduleCount - 1);

    while (b == a) {
        b = randInt2D(0, moduleCount - 1);
    }

    *m1 = a;
    *m2 = b;
}

// ------------------------------------------------------
// Apply a swap move using Grid + mod_x/mod_y arrays
// This is a *real* move (it changes the placement).
// Use only AFTER you've decided to accept the move in SA.
// ------------------------------------------------------
void applySwapMove2D(Grid* g, int mod_x[], int mod_y[], int m1, int m2) {
    if (!g) return;
    if (m1 == m2) return;

    int r1 = mod_x[m1];
    int c1 = mod_y[m1];

    int r2 = mod_x[m2];
    int c2 = mod_y[m2];

    if (r1 < 0 || r1 >= g->rows || c1 < 0 || c1 >= g->cols) return;
    if (r2 < 0 || r2 >= g->rows || c2 < 0 || c2 >= g->cols) return;

    // swap in grid
    int idx1 = r1 * g->cols + c1;
    int idx2 = r2 * g->cols + c2;

    int temp = g->cells[idx1];
    g->cells[idx1] = g->cells[idx2];
    g->cells[idx2] = temp;

    // update coordinates
    int tmpx = mod_x[m1];
    int tmpy = mod_y[m1];

    mod_x[m1] = mod_x[m2];
    mod_y[m1] = mod_y[m2];

    mod_x[m2] = tmpx;
    mod_y[m2] = tmpy;
}

// ------------------------------------------------------
// Count how many cells in the grid are EMPTY_CELL
// ------------------------------------------------------
int countEmptyCells(Grid* g) {
    if (!g) return 0;
    int totalCells = g->rows * g->cols;
    int count = 0;
    for (int i = 0; i < totalCells; i++) {
        if (g->cells[i] == EMPTY_CELL)
            count++;
    }
    return count;
}

// ------------------------------------------------------
// Randomly choose a module and a random *empty* cell.
//
// This function does NOT change the grid or mod_x/mod_y.
// It only outputs:
//   - module    : which module to move
//   - target_r  : target row
//   - target_c  : target column
//
// Return value:
//   1 = success (found empty target cell)
//   0 = failure (no empty cell available)
// ------------------------------------------------------
int generateRandomMoveToEmptyCell(Grid* g,
                                  int moduleCount,
                                  int* module,
                                  int* target_r,
                                  int* target_c)
{
    if (!g) return 0;

    int emptyCount = countEmptyCells(g);
    if (emptyCount == 0) {
        // No empty cells, cannot perform this type of move
        return 0;
    }

    // Choose a random module
    *module = randInt2D(0, moduleCount - 1);

    // Choose a random empty cell:
    // 1) randomly select a target index in [0, emptyCount-1]
    // 2) scan grid to locate that-th empty cell
    int targetIndex = randInt2D(0, emptyCount - 1);

    int currentEmptyIdx = 0;
    int totalCells = g->rows * g->cols;

    for (int idx = 0; idx < totalCells; idx++) {
        if (g->cells[idx] == EMPTY_CELL) {
            if (currentEmptyIdx == targetIndex) {
                // decode (r,c) from idx
                int r = idx / g->cols;
                int c = idx % g->cols;

                *target_r = r;
                *target_c = c;
                return 1;
            }
            currentEmptyIdx++;
        }
    }

    // Should not reach here
    return 0;
}
