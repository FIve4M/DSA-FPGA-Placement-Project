#include <cstdlib>
#include <cstdio>
#include "grid.h"

// ----------------------------------------------------
// Allocate and initialize a Grid structure
// ----------------------------------------------------
Grid* initGrid(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;

    Grid* g = new Grid;
    g->rows = rows;
    g->cols = cols;

    // allocate linear array
    g->cells = new int[rows * cols];

    // initialize to EMPTY_CELL
    for (int i = 0; i < rows * cols; i++) {
        g->cells[i] = EMPTY_CELL;
    }

    return g;
}

// ----------------------------------------------------
// Free memory used by grid
// ----------------------------------------------------
void freeGrid(Grid* g) {
    if (!g) return;
    if (g->cells) delete[] g->cells;
    delete g;
}

// ----------------------------------------------------
// Set all cells to EMPTY_CELL
// ----------------------------------------------------
void clearGrid(Grid* g) {
    if (!g) return;
    int n = g->rows * g->cols;
    for (int i = 0; i < n; i++) g->cells[i] = EMPTY_CELL;
}

// ----------------------------------------------------
// Check occupancy
// ----------------------------------------------------
int isOccupied(Grid* g, int r, int c) {
    if (!g) return 0;
    if (r < 0 || r >= g->rows || c < 0 || c >= g->cols) return 0;
    return (g->cells[r * g->cols + c] != EMPTY_CELL);
}

// ----------------------------------------------------
// Get module at cell
// ----------------------------------------------------
int getModuleAt(Grid* g, int r, int c) {
    if (!g) return EMPTY_CELL;
    if (r < 0 || r >= g->rows || c < 0 || c >= g->cols) return EMPTY_CELL;
    return g->cells[r * g->cols + c];
}

// ----------------------------------------------------
// Place module at (r,c)
// Overwrites any previous occupant — caller must keep mod_x/mod_y in sync
// ----------------------------------------------------
void placeModuleAt(Grid* g, int module, int r, int c) {
    if (!g) return;
    if (r < 0 || r >= g->rows || c < 0 || c >= g->cols) return;
    g->cells[r * g->cols + c] = module;
}

// ----------------------------------------------------
// Remove module at (r,c)
// ----------------------------------------------------
void removeModuleAt(Grid* g, int r, int c) {
    if (!g) return;
    if (r < 0 || r >= g->rows || c < 0 || c >= g->cols) return;
    g->cells[r * g->cols + c] = EMPTY_CELL;
}

// ----------------------------------------------------
// Swap two modules given module ids m1 and m2, and the mod_x/mod_y arrays.
// Assumes mod_x[]/mod_y[] contain current positions of modules.
// After swap, both grid and mod arrays are updated.
// ----------------------------------------------------
void swapModules2D(Grid* g, int mod_x[], int mod_y[], int m1, int m2) {
    if (!g) return;
    if (m1 == m2) return;

    int r1 = mod_x[m1];
    int c1 = mod_y[m1];

    int r2 = mod_x[m2];
    int c2 = mod_y[m2];

    // bounds check
    if (r1 < 0 || r1 >= g->rows || c1 < 0 || c1 >= g->cols) return;
    if (r2 < 0 || r2 >= g->rows || c2 < 0 || c2 >= g->cols) return;

    // swap in grid
    int idx1 = r1 * g->cols + c1;
    int idx2 = r2 * g->cols + c2;

    int temp = g->cells[idx1];
    g->cells[idx1] = g->cells[idx2];
    g->cells[idx2] = temp;

    // update module location arrays
    int tmpx = mod_x[m1];
    int tmpy = mod_y[m1];

    mod_x[m1] = mod_x[m2];
    mod_y[m1] = mod_y[m2];

    mod_x[m2] = tmpx;
    mod_y[m2] = tmpy;
}

// ----------------------------------------------------
// Move a module to (new_r, new_c). This overwrites target cell.
// Updates grid and mod_x/mod_y arrays.
// If the target cell contained another module, that module is overwritten
// (caller should handle swapping if desired).
// ----------------------------------------------------
void moveModuleTo(Grid* g, int mod_x[], int mod_y[], int module, int new_r, int new_c) {
    if (!g) return;

    if (new_r < 0 || new_r >= g->rows || new_c < 0 || new_c >= g->cols) return;

    // current position
    int cur_r = mod_x[module];
    int cur_c = mod_y[module];

    // set new cell to module (overwrite)
    g->cells[new_r * g->cols + new_c] = module;

    // clear old cell if it was within bounds and still held this module
    if (cur_r >= 0 && cur_r < g->rows && cur_c >= 0 && cur_c < g->cols) {
        int idxOld = cur_r * g->cols + cur_c;
        if (g->cells[idxOld] == module) {
            g->cells[idxOld] = EMPTY_CELL;
        }
    }

    // update module arrays
    mod_x[module] = new_r;
    mod_y[module] = new_c;
}

// ----------------------------------------------------
// Print grid (rows x cols) showing module ids or -1 for empty
// ----------------------------------------------------
void printGrid(Grid* g) {
    if (!g) return;

    printf("\n=== GRID (%d x %d) ===\n", g->rows, g->cols);
    for (int r = 0; r < g->rows; r++) {
        for (int c = 0; c < g->cols; c++) {
            int val = g->cells[r * g->cols + c];
            if (val == EMPTY_CELL)
                printf(" . ");
            else
                printf("%2d ", val);
        }
        printf("\n");
    }
    printf("======================\n");
}

// ----------------------------------------------------
// Linear index helpers
// ----------------------------------------------------
int linearIndex(Grid* g, int r, int c) {
    if (!g) return -1;
    if (r < 0 || r >= g->rows || c < 0 || c >= g->cols) return -1;
    return r * g->cols + c;
}

void indexToRC(Grid* g, int idx, int *r, int *c) {
    if (!g || !r || !c) return;
    if (idx < 0 || idx >= g->rows * g->cols) {
        *r = -1; *c = -1;
        return;
    }
    *r = idx / g->cols;
    *c = idx % g->cols;
}
