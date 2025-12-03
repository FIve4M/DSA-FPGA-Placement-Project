#ifndef MOVE_H
#define MOVE_H

// Swap two module positions in the placement array
void swapModules(int placement[], int i, int j);
// Generate a random swap move (neighbor state)
void generateRandomSwap(int placement[], int moduleCount, int* out_i, int* out_j);
// Random integer helper
int randInt(int min, int max);
// Optional: move module to random location
void generateRandomMove(int placement[], int moduleCount, int* module, int* newPos);

#endif