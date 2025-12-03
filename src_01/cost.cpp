#include "cost.h"
#include <cstdio>

// -------------------------------------------------------
// Simple integer absolute value (avoid <cmath> overhead)
// -------------------------------------------------------
int absInt(int x) {
    return (x < 0) ? -x : x;
}

// -------------------------------------------------------
// Distance function for placement ordering
// distance = |position_i - position_j|
// -------------------------------------------------------
int moduleDistance(int placement[], int a, int b) {
    return absInt(placement[a] - placement[b]);
}

int computeCost(Netlist* net, int placement[]) {
    int total = 0;

    for (int module = 0; module < net->moduleCount; module++) {
        Node* curr = net->adj[module];

        while (curr != NULL) {
            int neighbor = curr->module;

            // Avoid double counting: only count (module, neighbor) where neighbor > module
            if (neighbor > module) {
                int dist = moduleDistance(placement, module, neighbor);
                total += dist;  // weight = 1 for all nets (can expand later)
            }

            curr = curr->next;
        }
    }

    return total;
}

// -------------------------------------------------------
// Incremental cost change when swapping placement[i] and placement[j]
// This speeds up SA dramatically
// -------------------------------------------------------
int computeDeltaCost(Netlist* net, int placement[], int i, int j) {
    
    int old_i_pos = placement[i];
    int old_j_pos = placement[j];

    int delta = 0;

    // We need to evaluate only adjacency lists of modules i and j
    int modulesToCheck[2] = { i, j };

    for (int k = 0; k < 2; k++) {
        int mod = modulesToCheck[k];

        Node* curr = net->adj[mod];

        while (curr != NULL) {
            int neigh = curr->module;

            if (neigh == i || neigh == j) {
                curr = curr->next;
                continue; // self-pair handled separately
            }

            // Old distance
            int oldDist = absInt(placement[mod] - placement[neigh]);

            // Simulate swap
            int newPosMod = (mod == i) ? old_j_pos : (mod == j) ? old_i_pos : placement[mod];
            int newPosNei = placement[neigh];

            int newDist = absInt(newPosMod - newPosNei);

            delta += (newDist - oldDist);

            curr = curr->next;
        }
    }

    // Handle direct i ↔ j cost
    int i_old = absInt(old_i_pos - old_j_pos);
    int i_new = absInt(old_j_pos - old_i_pos);
    delta += (i_new - i_old);

    return delta;
}
