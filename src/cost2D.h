#ifndef COST2D_H
#define COST2D_H

#include "netlist.h"
#include "grid.h"

// Manhattan distance for 2D placement
int manhattanDistance2D(int x1, int y1, int x2, int y2);

// Full cost of placement
int computeCost2D(Netlist* net, int mod_x[], int mod_y[]);

// Incremental delta cost for swapping two modules m1 and m2
int computeDeltaCostSwap2D(Netlist* net, int mod_x[], int mod_y[], int m1, int m2);

// Optional: delta cost for moving a module to a new position
int computeDeltaCostMove2D(Netlist* net, int mod_x[], int mod_y[], int module, int new_x, int new_y);

#endif
