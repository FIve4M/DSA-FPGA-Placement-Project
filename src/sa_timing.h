#ifndef SA_TIMING_H
#define SA_TIMING_H

#include "grid.h"
#include "netlist.h"

// Timing-aware 2D simulated annealing
// - g:       pointer to grid
// - net:     netlist (adjacency list)
// - mod_x/y: module -> (row, col) coordinates
// - moduleCount: number of modules
void simulatedAnnealing2D(Grid* g, Netlist* net, int mod_x[], int mod_y[], int moduleCount);

#endif
