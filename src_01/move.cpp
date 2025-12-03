#include <cstdlib>
#include <ctime>
#include "move.h"

// ---------------------------------------------------------
// Random integer in [min, max]
// ---------------------------------------------------------
int randInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}
// ---------------------------------------------------------
// Swap positions of modules i and j in the placement array
// ---------------------------------------------------------
void swapModules(int placement[], int i, int j) {
    int temp = placement[i];
    placement[i] = placement[j];
    placement[j] = temp;
}
// ---------------------------------------------------------
// Generate a random neighbor by swapping two modules
//
// Outputs:
//   out_i = first module index
//   out_j = second module index
// ---------------------------------------------------------
void generateRandomSwap(int placement[], int moduleCount, int* out_i, int* out_j) {
    // Pick two different indices
    int i = randInt(0, moduleCount - 1);
    int j = randInt(0, moduleCount - 1);
    while (j == i) {
        j = randInt(0, moduleCount - 1);
    }
    // return chosen indices
    *out_i = i;
    *out_j = j;
}
// ---------------------------------------------------------
// Optional: Move a module to a NEW random position
// Instead of swapping two modules.
// Rarely used in FPGA SA but included for flexibility.
// ---------------------------------------------------------
void generateRandomMove(int placement[], int moduleCount, int* module, int* newPos) {
    *module = randInt(0, moduleCount - 1);
    *newPos  = randInt(0, moduleCount - 1);
    // Ensure module isn't assigned same position
    while (*newPos == placement[*module]) {
        *newPos = randInt(0, moduleCount - 1);
    }
}
