#ifndef COST_H
#define COST_H

#include "netlist.h"

// Full cost computation
int computeCost(Netlist* net, int placement[]);

// Incremental cost computation for swapping two modules
int computeDeltaCost(Netlist* net, int placement[], int i, int j);

int absInt(int x);

#endif
