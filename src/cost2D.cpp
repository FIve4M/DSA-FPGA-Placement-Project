#include <cstdio>
#include "netlist.h"
#include "cost2D.h"

// ----------------------------------------------------------
// Manhattan distance |x1 - x2| + |y1 - y2|
// ----------------------------------------------------------
int manhattanDistance2D(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    if (dx < 0) dx = -dx;

    int dy = y1 - y2;
    if (dy < 0) dy = -dy;

    return dx + dy;
}

// ----------------------------------------------------------
// Full placement cost (no double-counting):
// For each module m, only count edges where neigh > m.
// ----------------------------------------------------------
int computeCost2D(Netlist* net, int mod_x[], int mod_y[]) {

    long long total = 0;

    for (int m = 0; m < net->moduleCount; m++) {
        Node* curr = net->adj[m];

        while (curr != NULL) {
            int neigh = curr->module;

            // Prevent double counting of undirected edge
            if (neigh > m) {
                int x1 = mod_x[m];
                int y1 = mod_y[m];
                int x2 = mod_x[neigh];
                int y2 = mod_y[neigh];

                total += manhattanDistance2D(x1, y1, x2, y2);
            }

            curr = curr->next;
        }
    }

    return (int)total;
}

// ----------------------------------------------------------
// Incremental delta-cost for swapping modules m1 and m2
//
// Only edges incident to m1 or m2 change.
// Adjacency contains directed edges but we only
// scan adj[m1] and adj[m2], so each affected edge
// is touched once per endpoint.
// ----------------------------------------------------------
int computeDeltaCostSwap2D(Netlist* net, int mod_x[], int mod_y[],
                           int m1, int m2)
{
    int old_x1 = mod_x[m1], old_y1 = mod_y[m1];
    int old_x2 = mod_x[m2], old_y2 = mod_y[m2];

    long long delta = 0;

    // neighbors of m1
    Node* curr = net->adj[m1];
    while (curr != NULL) {
        int neigh = curr->module;

        if (neigh == m2) {
            // distance between m1 and m2 is symmetric and
            // unchanged by swapping positions, so ignore.
            curr = curr->next;
            continue;
        }

        int xn = mod_x[neigh];
        int yn = mod_y[neigh];

        int oldDist = manhattanDistance2D(old_x1, old_y1, xn, yn);
        int newDist = manhattanDistance2D(old_x2, old_y2, xn, yn);

        delta += (newDist - oldDist);

        curr = curr->next;
    }

    // neighbors of m2
    curr = net->adj[m2];
    while (curr != NULL) {
        int neigh = curr->module;

        if (neigh == m1) {
            curr = curr->next;
            continue;
        }

        int xn = mod_x[neigh];
        int yn = mod_y[neigh];

        int oldDist = manhattanDistance2D(old_x2, old_y2, xn, yn);
        int newDist = manhattanDistance2D(old_x1, old_y1, xn, yn);

        delta += (newDist - oldDist);

        curr = curr->next;
    }

    return (int)delta;
}

// ----------------------------------------------------------
// Optional: delta cost for moving a module to a new cell.
// Not required if you only use swaps, but kept for flexibility.
// ----------------------------------------------------------
int computeDeltaCostMove2D(Netlist* net, int mod_x[], int mod_y[],
                           int module, int new_x, int new_y)
{
    int old_x = mod_x[module];
    int old_y = mod_y[module];

    long long delta = 0;

    Node* curr = net->adj[module];

    while (curr != NULL) {
        int neigh = curr->module;

        int xn = mod_x[neigh];
        int yn = mod_y[neigh];

        int oldDist = manhattanDistance2D(old_x, old_y, xn, yn);
        int newDist = manhattanDistance2D(new_x, new_y, xn, yn);

        delta += (newDist - oldDist);

        curr = curr->next;
    }

    return (int)delta;
}
