#include <cstdio>
#include <cmath>
#include <cstdlib>

#include "sa_timing.h"
#include "netlist.h"
#include "grid.h"
#include "cost2D.h"
#include "move2D.h"
#include "pq.h"

// ----------------------------------------------------------
// Metropolis acceptance function
// ----------------------------------------------------------
static int acceptMove2D(int delta, double T) {
    if (delta <= 0) return 1;

    double prob = exp(-((double)delta) / T);
    double r = (double)rand() / RAND_MAX;

    return (r < prob);
}

// ----------------------------------------------------------
// Local cost contribution of a module
// (sum of distances to all neighbors)
// ----------------------------------------------------------
static int computeModuleLocalCost2D(Netlist* net, int mod_x[], int mod_y[], int module) {
    int cost = 0;
    int x1 = mod_x[module];
    int y1 = mod_y[module];

    Node* curr = net->adj[module];

    while (curr != NULL) {
        int neigh = curr->module;
        int x2 = mod_x[neigh];
        int y2 = mod_y[neigh];

        cost += manhattanDistance2D(x1, y1, x2, y2);

        curr = curr->next;
    }

    return cost;
}

// ----------------------------------------------------------
// Build Priority Queue of modules sorted by local cost
// Higher cost = more critical
// ----------------------------------------------------------
static void buildModulePriorityQueue(Netlist* net,
                                     int mod_x[], int mod_y[],
                                     PriorityQueue* pq)
{
    initPQ(pq);

    for (int m = 0; m < net->moduleCount; m++) {
        int localCost = computeModuleLocalCost2D(net, mod_x, mod_y, m);
        pqInsert(pq, m, localCost);
    }
}

// ----------------------------------------------------------
// Main 2D Simulated Annealing routine
// ----------------------------------------------------------
void simulatedAnnealing2D(Grid* g, Netlist* net,
                          int mod_x[], int mod_y[],
                          int moduleCount)
{
    // SA parameters (tune if needed)
    double T      = 1000.0;
    double Tmin   = 0.1;
    double alpha  = 0.95;
    int iterationsPerT = moduleCount;  // inner loop per temperature

    int currentCost = computeCost2D(net, mod_x, mod_y);
    int bestCost    = currentCost;

    // store best placement
    int* best_x = new int[moduleCount];
    int* best_y = new int[moduleCount];

    for (int i = 0; i < moduleCount; i++) {
        best_x[i] = mod_x[i];
        best_y[i] = mod_y[i];
    }

    printf("Initial 2D Cost: %d\n", currentCost);

    PriorityQueue pq;

    while (T > Tmin) {

        // Rebuild criticality PQ for this temperature
        buildModulePriorityQueue(net, mod_x, mod_y, &pq);

        for (int iter = 0; iter < iterationsPerT; iter++) {

            int m1, m2;

            // 50% chance: PQ-guided (critical module)
            int usePQ = (rand() % 100) < 50;

            if (usePQ && !pqIsEmpty(&pq)) {
                PQNode top = pqExtractMax(&pq);
                m1 = top.moduleID;

                // choose random partner
                m2 = randInt2D(0, moduleCount - 1);
                while (m2 == m1) {
                    m2 = randInt2D(0, moduleCount - 1);
                }
            } else {
                // fully random swap
                generateRandomModulePair(moduleCount, &m1, &m2);
            }

            int delta = computeDeltaCostSwap2D(net, mod_x, mod_y, m1, m2);

            if (acceptMove2D(delta, T)) {
                // apply swap
                applySwapMove2D(g, mod_x, mod_y, m1, m2);
                currentCost += delta;

                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    for (int i = 0; i < moduleCount; i++) {
                        best_x[i] = mod_x[i];
                        best_y[i] = mod_y[i];
                    }
                }
            }
        }

        printf("T = %.2f, Current Cost = %d, Best = %d\n", T, currentCost, bestCost);
        T = T * alpha;
    }

    // restore best placement
    for (int i = 0; i < moduleCount; i++) {
        mod_x[i] = best_x[i];
        mod_y[i] = best_y[i];
    }

    // rebuild grid according to best placement
    clearGrid(g);
    for (int m = 0; m < moduleCount; m++) {
        int r = mod_x[m];
        int c = mod_y[m];
        placeModuleAt(g, m, r, c);
    }

    printf("Final Best 2D Cost: %d\n", bestCost);

    delete[] best_x;
    delete[] best_y;
}
