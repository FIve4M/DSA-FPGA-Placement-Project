#include <cstdio>
#include <cstdlib>
#include "parser.h"
#include "netlist.h"

// --------------------------------------------------------------
// Parse a netlist file into the adjacency-list Netlist structure
//
// Format:
//   <moduleCount>
//   <netCount>
//   For each net:
//       <k modules> <m1> <m2> ... <mk>
// --------------------------------------------------------------
Netlist* parseNetlistFile(const char* filename, int* moduleCount) {

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }
    int netCount = 0;
    // Read module count
    if (fscanf(file, "%d", moduleCount) != 1) {
        printf("Error: Could not read module count\n");
        fclose(file);
        return NULL;
    }
    // Read net count
    if (fscanf(file, "%d", &netCount) != 1) {
        printf("Error: Could not read net count\n");
        fclose(file);
        return NULL;
    }
    // Initialize netlist graph
    Netlist* net = initNetlist(*moduleCount);
    // Temporary array to store modules in a net
    int modulesInNet[256];   // supports nets up to size 256
    for (int n = 0; n < netCount; n++) {
        int k = 0;
        if (fscanf(file, "%d", &k) != 1) {
            printf("Error: Could not read number of modules in net\n");
            fclose(file);
            return net;
        }
        // Read the next k module IDs
        for (int i = 0; i < k; i++) {
            fscanf(file, "%d", &modulesInNet[i]);
        }
        // Add this net to the adjacency list
        addNet(net, modulesInNet, k);
    }

    fclose(file);
    return net;
}
