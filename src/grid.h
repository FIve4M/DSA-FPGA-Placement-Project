#ifndef GRID_H
#define GRID_H

// Special value for empty cell
#define EMPTY_CELL -1

// Grid structure
struct Grid {
    int rows;
    int cols;
    int *cells;      // linear array size rows*cols, cells[r*cols + c] -> module id or EMPTY_CELL
};

// Initialize grid structure (allocates memory)
// Returns pointer to Grid, or nullptr on failure
Grid* initGrid(int rows, int cols);

// Free grid memory
void freeGrid(Grid* g);

// Set all cells to EMPTY_CELL
void clearGrid(Grid* g);

// Check if a cell is occupied (returns 1 if occupied, 0 otherwise)
int isOccupied(Grid* g, int r, int c);

// Get module at cell (returns module id or EMPTY_CELL)
int getModuleAt(Grid* g, int r, int c);

// Place module at (r,c). If that cell is occupied, this will overwrite it.
// caller should maintain mod_x/mod_y arrays consistently.
void placeModuleAt(Grid* g, int module, int r, int c);

// Remove module from (r,c) — sets cell to EMPTY_CELL
void removeModuleAt(Grid* g, int r, int c);

// Swap positions of two modules given their module ids and mod_x/mod_y arrays.
// mod_x[module] and mod_y[module] must hold current coordinates before calling.
void swapModules2D(Grid* g, int mod_x[], int mod_y[], int m1, int m2);

// Move a module to a target cell (r,c). Updates grid and mod_x/mod_y.
// If target cell occupied, caller decides whether to swap or fail; this function overwrites.
void moveModuleTo(Grid* g, int mod_x[], int mod_y[], int module, int new_r, int new_c);

// Print grid in ASCII format (shows module ids; -1 for empty)
void printGrid(Grid* g);

// Helpers: convert linear index <-> r,c
int linearIndex(Grid* g, int r, int c);
void indexToRC(Grid* g, int idx, int *r, int *c);

#endif
