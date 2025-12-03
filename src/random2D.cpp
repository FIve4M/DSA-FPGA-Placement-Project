#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "random2D.h"
#include "grid.h"

void randomInitialPlacement2D(Grid* g, int moduleCount, int mod_x[], int mod_y[]) {
    if (!g) {
        printf("Error: Grid is NULL\n");
        return;
    }

    int totalCells = g->rows * g->cols;

    if (moduleCount > totalCells) {
        printf("Error: Not enough grid cells for all modules\n");
        return;
    }

    // Seed RNG (do once ideally)
    srand(time(NULL));

    // Temporary array of all grid indices (0..rows*cols - 1)
    int *indices = new int[totalCells];
    for (int i = 0; i < totalCells; i++) {
        indices[i] = i;
    }

    // Fisher–Yates shuffle of available grid positions
    for (int i = totalCells - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Assign each module to one of the first 'moduleCount' cells
    for (int m = 0; m < moduleCount; m++) {

        int idx = indices[m];
        int r = idx / g->cols;
        int c = idx % g->cols;

        // Assign into grid
        g->cells[idx] = m;

        // Update module lookup tables
        mod_x[m] = r;
        mod_y[m] = c;
    }

    delete[] indices;
}
