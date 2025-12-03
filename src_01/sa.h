#ifndef SA_H
#define SA_H

#include "netlist.h"

void simulatedAnnealing(Netlist* net, int placement[], int moduleCount);

int acceptMove(int delta, double T);

#endif
