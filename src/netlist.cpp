#include <cstdio>
#include <cstdlib>
#include "netlist.h"

// ------------------------------------------------------------
// Allocate empty netlist with adjacency lists initialized to NULL
// ------------------------------------------------------------
Netlist* initNetlist(int moduleCount) {
    Netlist* net = new Netlist;
    net->moduleCount = moduleCount;
    net->adj = new Node*[moduleCount];

    for (int i = 0; i < moduleCount; i++)
        net->adj[i] = nullptr;

    return net;
}

// ------------------------------------------------------------
// Add edge a → b (directed). Caller must add both directions if needed.
// ------------------------------------------------------------
void addEdge(Netlist* net, int a, int b) {
    if (!net) return;
    if (a < 0 || a >= net->moduleCount) return;
    if (b < 0 || b >= net->moduleCount) return;

    Node* n = allocNode();
    n->module = b;
    n->next = net->adj[a];
    net->adj[a] = n;
}

// ------------------------------------------------------------
// Free entire adjacency list
// ------------------------------------------------------------
void freeNetlist(Netlist* net) {
    if (!net) return;
    delete[] net->adj;
    delete net;
}


// ------------------------------------------------------------
// Optional: print adjacency list (debug)
// ------------------------------------------------------------
void printNetlist(Netlist* net) {
    if (!net) return;

    printf("\n=== Netlist Adjacency ===\n");
    for (int i = 0; i < net->moduleCount; i++) {
        printf("%d: ", i);
        Node* curr = net->adj[i];
        while (curr) {
            printf("%d ", curr->module);
            curr = curr->next;
        }
        printf("\n");
    }
    printf("=========================\n");
}

// ------------------------------------------------------------
// Histogram buckets: 0..19, and >=20
// ------------------------------------------------------------
void printNetlistStats(Netlist* net) {
    if (!net) {
        printf("ERROR: Netlist is NULL.\n");
        return;
    }

    int moduleCount = net->moduleCount;
    long long totalDegree = 0;
    int zeroDegreeModules = 0;
    int maxDegree = 0;

    int hist[21] = {0};  // bucket 20 = overflow

    for (int m = 0; m < moduleCount; m++) {
        Node* curr = net->adj[m];
        int degree = 0;

        while (curr) {
            degree++;
            curr = curr->next;
        }

        totalDegree += degree;

        if (degree == 0) zeroDegreeModules++;
        if (degree > maxDegree) maxDegree = degree;

        if (degree < 20)
            hist[degree]++;
        else
            hist[20]++;
    }

    double avgDegree = (moduleCount > 0)
        ? (double)totalDegree / moduleCount
        : 0.0;

    printf("\n=== Netlist Statistics ===\n");
    printf(" Total Modules:        %d\n", moduleCount);
    printf(" Zero-degree Modules:  %d\n", zeroDegreeModules);
    printf(" Average Degree:       %.2f\n", avgDegree);
    printf(" Maximum Degree:       %d\n", maxDegree);
    printf(" Total Edges:          %lld\n", totalDegree / 2);
    printf("\nDegree Histogram:\n");

    for (int d = 0; d <= 20; d++) {
        if (d < 20)
            printf(" degree %2d: %d modules\n", d, hist[d]);
        else
            printf(" degree >=20: %d modules\n", hist[d]);
    }

    printf("===========================\n\n");
}

// ------------------------------------------------------------
// Integrity Checker
// - invalid IDs
// - duplicates
// - self-loops
// ------------------------------------------------------------

// simple non-STL hash mechanism
static int seen[20000];
static int seenTag = 1;

void checkNetlistIntegrity(Netlist* net) {
    if (!net) {
        printf("ERROR: Netlist is NULL.\n");
        return;
    }

    int moduleCount = net->moduleCount;
    int errors = 0;

    printf("\n=== Checking Netlist Integrity ===\n");

    for (int m = 0; m < moduleCount; m++) {

        Node* curr = net->adj[m];

        seenTag++;   // new tag for this module

        while (curr) {
            int neigh = curr->module;

            // Out of bounds
            if (neigh < 0 || neigh >= moduleCount) {
                printf("ERROR: %d has invalid neighbor ID %d\n", m, neigh);
                errors++;
            }

            // Self-loop
            if (neigh == m) {
                printf("ERROR: Self-loop found at module %d\n", m);
                errors++;
            }

            // Duplicates detection
            if (seen[neigh] == seenTag) {
                printf("ERROR: Duplicate edge: %d ↔ %d\n", m, neigh);
                errors++;
            } else {
                seen[neigh] = seenTag;
            }

            curr = curr->next;
        }
    }

    if (errors == 0)
        printf("Netlist Integrity Check: PASSED.\n");
    else
        printf("Netlist Integrity Check: FAILED (%d errors).\n", errors);

    printf("==================================\n\n");
}
// ===========================================================
// MEMORY POOL IMPLEMENTATION
// ===========================================================

static NodePool GLOBAL_POOL = {nullptr, 0, 0};

void initNodePool(int capacity) {
    GLOBAL_POOL.pool = (Node*)malloc(sizeof(Node) * capacity);
    GLOBAL_POOL.capacity = capacity;
    GLOBAL_POOL.used = 0;

    if (!GLOBAL_POOL.pool) {
        printf("ERROR: Failed to allocate node pool.\n");
        exit(1);
    }
}

Node* allocNode() {
    if (GLOBAL_POOL.used >= GLOBAL_POOL.capacity) {
        printf("ERROR: Node pool exhausted (%d nodes).\n", GLOBAL_POOL.capacity);
        exit(1);
    }

    Node* n = &GLOBAL_POOL.pool[GLOBAL_POOL.used++];
    // no need to zero — we assign fields
    return n;
}

void freeNodePool() {
    if (GLOBAL_POOL.pool) {
        free(GLOBAL_POOL.pool);
        GLOBAL_POOL.pool = nullptr;
        GLOBAL_POOL.capacity = 0;
        GLOBAL_POOL.used = 0;
    }
}
