#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "csv_parser.h"
#include "netlist.h"
#include "grid.h"
#include "random2D.h"
#include "cost2D.h"
#include "sa_timing.h"

int main() {

    srand((unsigned int)time(NULL));

    // ---------------------------------------------------------
    // 1) Initialize adjacency memory pool
    //    (adjust capacity if you know approx edges)
    // ---------------------------------------------------------
    initNodePool(1000000);   // example: 1 million adjacency nodes

    // ---------------------------------------------------------
    // 2) Load CSV netlist
    // ---------------------------------------------------------
    int moduleCount = 0;
    const char* filename = "tests/nets_5k.csv";

    Netlist* net = parseCSVNetlist(filename, &moduleCount);
    if (!net) {
        printf("Failed to parse netlist file: %s\n", filename);
        freeNodePool();
        return 1;
    }

    printf("Netlist loaded. Module count = %d\n", moduleCount);

    printNetlistStats(net);
    checkNetlistIntegrity(net);

    // ---------------------------------------------------------
    // 3) Create FPGA grid (must have enough cells)
    // ---------------------------------------------------------
    int rows = 80;
    int cols = 80;

    if (rows * cols < moduleCount) {
        printf("ERROR: Grid too small for %d modules (%d cells only)\n",
               moduleCount, rows * cols);
        freeNetlist(net);
        freeNodePool();
        return 1;
    }

    Grid* g = initGrid(rows, cols);
    if (!g) {
        printf("ERROR: Could not allocate grid.\n");
        freeNetlist(net);
        freeNodePool();
        return 1;
    }

    // ---------------------------------------------------------
    // 4) Allocate placement coordinate arrays
    // ---------------------------------------------------------
    int* mod_x = new int[moduleCount];
    int* mod_y = new int[moduleCount];

    for (int i = 0; i < moduleCount; i++) {
        mod_x[i] = -1;
        mod_y[i] = -1;
    }

    // ---------------------------------------------------------
    // 5) Random initial placement
    // ---------------------------------------------------------
    clearGrid(g);
    randomInitialPlacement2D(g, moduleCount, mod_x, mod_y);

    printf("\nInitial placement grid:\n");
    printGrid(g);

    int initialCost = computeCost2D(net, mod_x, mod_y);
    printf("Initial 2D cost = %d\n", initialCost);

    // ---------------------------------------------------------
    // 6) Run Simulated Annealing
    // ---------------------------------------------------------
    simulatedAnnealing2D(g, net, mod_x, mod_y, moduleCount);

    // ---------------------------------------------------------
    // 7) Show final placement
    // ---------------------------------------------------------
    printf("\nFinal placement grid:\n");
    printGrid(g);

    int finalCost = computeCost2D(net, mod_x, mod_y);
    printf("Final 2D cost = %d\n", finalCost);

    // ---------------------------------------------------------
    // 8) Cleanup
    // ---------------------------------------------------------
    delete[] mod_x;
    delete[] mod_y;

    freeGrid(g);
    freeNetlist(net);
    freeNodePool();

    return 0;
}
