#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "netlist.h"

static int parseBlockID(const char* token) {
    // token looks like "B_4186"
    if (token[0] != 'B' || token[1] != '_') return -1;
    return atoi(token + 2);
}

Netlist* parseNetlistFile(const char* filename, int* moduleCountOut) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open netlist file %s\n", filename);
        return nullptr;
    }

    // Maximum block id encountered
    int maxModule = -1;

    // Temporary storage for lines
    char line[4096];

    // Skip header lines like:
    // "Net_5k"
    // "NetName   Weight   ConnectedBlocks (Space Separated)"
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    // First pass: find maximum module ID
    while (fgets(line, sizeof(line), fp)) {
        char* saveptr;
        char* netname = strtok_r(line, "\t", &saveptr);
        char* weight  = strtok_r(nullptr, "\t", &saveptr);
        char* blocks  = strtok_r(nullptr, "\t", &saveptr);

        if (!blocks) continue;

        char* bsave;
        char* token = strtok_r(blocks, " ", &bsave);

        while (token) {
            int id = parseBlockID(token);
            if (id > maxModule) maxModule = id;
            token = strtok_r(nullptr, " ", &bsave);
        }
    }

    rewind(fp);
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    int moduleCount = maxModule + 1;
    *moduleCountOut = moduleCount;

    Netlist* netlist = initNetlist(moduleCount);

    // Second pass: build edges
    while (fgets(line, sizeof(line), fp)) {
        char* saveptr;
        strtok_r(line, "\t", &saveptr); // netname
        strtok_r(nullptr, "\t", &saveptr); // weight
        char* blocks = strtok_r(nullptr, "\t", &saveptr);
        if (!blocks) continue;

        // parse block list into array
        int modules[512];
        int count = 0;

        char* bsave;
        char* token = strtok_r(blocks, " ", &bsave);

        while (token) {
            int id = parseBlockID(token);
            if (id >= 0) modules[count++] = id;
            token = strtok_r(nullptr, " ", &bsave);
        }

        // Build complete clique adjacency
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                addEdge(netlist, modules[i], modules[j]);
                addEdge(netlist, modules[j], modules[i]);
            }
        }
    }

    fclose(fp);
    return netlist;
}
