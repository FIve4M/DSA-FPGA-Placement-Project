#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdio>

#include "sa.h"
#include "cost.h"
#include "move.h"
#include "netlist.h"

// -----------------------------------------------------------
// Accept move using Metropolis criterion
// Returns 1 if accepted, 0 if rejected
// -----------------------------------------------------------
int acceptMove(int delta, double T) {
    if (delta < 0) {
        return 1;  // Always accept improving move
    }

    double prob = exp(-((double)delta) / T);
    double r = (double)rand() / RAND_MAX;

    return (r < prob);
}

// -----------------------------------------------------------
// Main Simulated Annealing routine
// Uses:
//  - computeCost()
//  - computeDeltaCost()
//  - generateRandomSwap()
//  - swapModules()
// -----------------------------------------------------------
void simulatedAnnealing(Netlist* net, int placement[], int moduleCount) {

    // ---------------------------
    // SA parameters (adjustable)
    // ---------------------------
    double T      = 1000.0;     // initial temperature
    double Tmin   = 0.001;      // stopping threshold
    double alpha  = 0.97;       // cooling rate
    int iterationsPerT = 2000;  // inner loop

    srand(time(NULL));

    // -----------------------------------------------------
    // Compute initial cost
    // -----------------------------------------------------
    int currentCost = computeCost(net, placement);

    // Best solution so far
    int* bestPlacement = new int[moduleCount];
    for (int i = 0; i < moduleCount; i++)
        bestPlacement[i] = placement[i];

    int bestCost = currentCost;

    printf("Initial Cost: %d\n", currentCost);

    // -----------------------------------------------------
    // Main SA loop
    // -----------------------------------------------------
    while (T > Tmin) {

        for (int k = 0; k < iterationsPerT; k++) {

            int i, j;

            // Pick random swap
            generateRandomSwap(placement, moduleCount, &i, &j);

            // Compute delta cost for swapping i,j
            int delta = computeDeltaCost(net, placement, i, j);

            // Accept or reject
            if (acceptMove(delta, T)) {

                // Apply swap
                swapModules(placement, i, j);
                currentCost += delta;

                // Track best solution
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    for (int a = 0; a < moduleCount; a++)
                        bestPlacement[a] = placement[a];
                }

            }
        }

        // Cool down
        T = T * alpha;
    }

    printf("Final Best Cost: %d\n", bestCost);

    // Copy best placement into output
    for (int i = 0; i < moduleCount; i++)
        placement[i] = bestPlacement[i];

    delete[] bestPlacement;
}
void printPlacement(int placement[], int moduleCount) {

    printf("\n=== PLACEMENT MAP ===\n");
    for (int i = 0; i < moduleCount; i++) {
        printf("Module %d -> Position %d\n", i, placement[i]);
    }
    printf("======================\n");
}