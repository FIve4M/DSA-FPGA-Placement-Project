#ifndef PQ_H
#define PQ_H

#define MAX_NETS 200000   // treat as MAX_MODULES

struct PQNode {
    int moduleID;   // previously netID
    int priority;   // local cost (higher = more critical)
};

struct PriorityQueue {
    PQNode heap[MAX_NETS];
    int size;
};

void initPQ(PriorityQueue* pq);
void pqInsert(PriorityQueue* pq, int moduleID, int priority);
PQNode pqExtractMax(PriorityQueue* pq);
PQNode pqPeek(PriorityQueue* pq);
int pqIsEmpty(PriorityQueue* pq);

#endif
