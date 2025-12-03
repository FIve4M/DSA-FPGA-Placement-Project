#include "pq.h"
#include <cstdio>

// -------------------------------------------------------------
// Initialize PQ
// -------------------------------------------------------------
void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

// -------------------------------------------------------------
// Swap two PQNode elements
// -------------------------------------------------------------
void swapNodes(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

// -------------------------------------------------------------
// Heapify Up (bubble up)
// -------------------------------------------------------------
void heapifyUp(PriorityQueue* pq, int idx) {
    while (idx > 1) {
        int parent = idx / 2;
        if (pq->heap[parent].priority >= pq->heap[idx].priority)
            break;

        swapNodes(&pq->heap[parent], &pq->heap[idx]);
        idx = parent;
    }
}

// -------------------------------------------------------------
// Insert module into PQ
// -------------------------------------------------------------
void pqInsert(PriorityQueue* pq, int moduleID, int priority) {

    if (pq->size + 1 >= MAX_NETS) {
        printf("Priority Queue overflow!\n");
        return;
    }

    pq->size++;
    pq->heap[pq->size].moduleID = moduleID;
    pq->heap[pq->size].priority = priority;

    heapifyUp(pq, pq->size);
}

// -------------------------------------------------------------
// Heapify down after extracting max
// -------------------------------------------------------------
void heapifyDown(PriorityQueue* pq, int idx) {
    while (true) {
        int left = idx * 2;
        int right = left + 1;
        int largest = idx;

        if (left <= pq->size &&
            pq->heap[left].priority > pq->heap[largest].priority)
            largest = left;

        if (right <= pq->size &&
            pq->heap[right].priority > pq->heap[largest].priority)
            largest = right;

        if (largest == idx)
            break;

        swapNodes(&pq->heap[idx], &pq->heap[largest]);
        idx = largest;
    }
}

// -------------------------------------------------------------
// Extract max-priority MODULE
// -------------------------------------------------------------
PQNode pqExtractMax(PriorityQueue* pq) {
    if (pq->size == 0) {
        PQNode empty = {-1, -1};
        return empty;
    }

    PQNode maxNode = pq->heap[1];

    pq->heap[1] = pq->heap[pq->size];
    pq->size--;

    heapifyDown(pq, 1);

    return maxNode;
}

// -------------------------------------------------------------
// Peek at highest-priority module
// -------------------------------------------------------------
PQNode pqPeek(PriorityQueue* pq) {
    if (pq->size == 0) {
        PQNode empty = {-1, -1};
        return empty;
    }
    return pq->heap[1];
}

// -------------------------------------------------------------
// Check if empty
// -------------------------------------------------------------
int pqIsEmpty(PriorityQueue* pq) {
    return (pq->size == 0);
}
