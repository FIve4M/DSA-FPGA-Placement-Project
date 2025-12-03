#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "csv_parser.h"
#include "netlist.h"

// Extract integer from "B_4186"
static int parseBlockID(const char* token) {
    if (token[0] != 'B' || token[1] != '_')
        return -1;
    return atoi(token + 2);
}

// ========================================================
// FIRST PASS — determine max block ID
// ========================================================
static int scanMaxModuleID(FILE* fp) {
    char line[8192];
    int maxID = -1;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == 0)
            continue;

        // Split line into tokens
        char* save;
        char* field1 = strtok_r(line, ",\t", &save);  // NetName
        char* field2 = strtok_r(NULL, ",\t", &save);  // Weight
        char* field3 = strtok_r(NULL, "\n", &save);   // Blocks

        if (!field3) continue;

        char* token;
        char* bsave;
        token = strtok_r(field3, " ", &bsave); // space-separated blocks

        while (token) {
            int id = parseBlockID(token);
            if (id > maxID) maxID = id;
            token = strtok_r(NULL, " ", &bsave);
        }
    }
    return maxID;
}

// ========================================================
// SECOND PASS — Build adjacency list
// ========================================================
Netlist* parseCSVNetlist(const char* filename, int* moduleCountOut) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: Cannot open CSV netlist: %s\n", filename);
        return nullptr;
    }

    // -------------------------------------------------------
    // 1. First pass — determine highest module ID
    // -------------------------------------------------------
    int maxID = scanMaxModuleID(fp);
    if (maxID < 0) {
        printf("ERROR: No valid block IDs found.\n");
        fclose(fp);
        return nullptr;
    }

    int moduleCount = maxID + 1;
    *moduleCountOut = moduleCount;

    rewind(fp);

    // NOTE: YOU MUST CALL initNodePool BEFORE THIS FUNCTION
    Netlist* net = initNetlist(moduleCount);

    char line[8192];

    // -------------------------------------------------------
    // 2. Second pass — actually build adjacency
    // -------------------------------------------------------
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == 0)
            continue;

        char* save;
        strtok_r(line, ",\t", &save);   // net name
        strtok_r(NULL, ",\t", &save);   // weight
        char* blocks = strtok_r(NULL, "\n", &save);

        if (!blocks) continue;

        // List of block IDs for this net
        int modules[2048];
        int count = 0;

        char* bsave;
        char* token = strtok_r(blocks, " ", &bsave);  // space-separated list

        while (token) {
            int id = parseBlockID(token);
            if (id >= 0) modules[count++] = id;
            token = strtok_r(NULL, " ", &bsave);
        }

        // Add all pairwise edges (clique expansion)
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                addEdge(net, modules[i], modules[j]);
                addEdge(net, modules[j], modules[i]);
            }
        }
    }

    fclose(fp);
    return net;
}
