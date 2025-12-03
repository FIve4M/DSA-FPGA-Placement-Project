#ifndef NETLIST_H
#define NETLIST_H

struct Node {
    int module;
    Node* next;
};

// ============================
// Memory Pool for adjacency nodes
// ============================
struct NodePool {
    Node* pool;
    int capacity;
    int used;
};

void initNodePool(int capacity);
Node* allocNode();
void freeNodePool();

// ============================
// Existing netlist structures
// ============================
struct Netlist {
    int moduleCount;
    Node** adj;
};

Netlist* initNetlist(int moduleCount);
void addEdge(Netlist* net, int a, int b);
void freeNetlist(Netlist* net);

void printNetlist(Netlist* net);
void printNetlistStats(Netlist* net);
void checkNetlistIntegrity(Netlist* net);

#endif
