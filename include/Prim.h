#ifndef PRIM_H
#define PRIM_H

#include <iostream>
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

struct MSTEdge {
    int from;
    int to;
    int weight;
};

struct MSTResult {
    MSTEdge* edges;
    int edgeCount;
    int totalWeight;

    MSTResult(int maxEdges) : edgeCount(0), totalWeight(0) {
        edges = new MSTEdge[maxEdges];
    }

    ~MSTResult() {
        delete[] edges;
    }

    void print() const {
        std::cout << "MST (Prim):\n";
        for (int i = 0; i < edgeCount; i++) {
            std::cout << "  " << edges[i].from << " -- " << edges[i].to
                      << "  weight: " << edges[i].weight << "\n";
        }
        std::cout << "Total weight: " << totalWeight << "\n";
    }
};

int minKey(int* key, bool* inMST, int vertices) {
    int min = -1;
    for (int v = 0; v < vertices; v++) {
        if (!inMST[v] && key[v] != -1) {
            if (min == -1 || key[v] < key[min]) {
                min = v;
            }
        }
    }
    return min;
}

MSTResult* primMatrix(const IncidenceMatrix& graph) {
    int v = graph.getVertices();
    int e = graph.getEdges();

    int* key = new int[v];
    int* parent = new int[v];
    bool* inMST = new bool[v];

    for (int i = 0; i < v; i++) {
        key[i] = -1;
        parent[i] = -1;
        inMST[i] = false;
    }

    key[0] = 0;

    for (int iter = 0; iter < v; iter++) {
        int u = minKey(key, inMST, v);
        if (u == -1) break;

        inMST[u] = true;

        for (int j = 0; j < e; j++) {
            if (graph.getValue(u, j) == 0) continue;

            int neighbor = -1;
            for (int i = 0; i < v; i++) {
                if (i != u && graph.getValue(i, j) != 0) {
                    neighbor = i;
                    break;
                }
            }

            if (neighbor == -1 || inMST[neighbor]) continue;

            int w = graph.getValue(u, j);
            if (key[neighbor] == -1 || w < key[neighbor]) {
                key[neighbor] = w;
                parent[neighbor] = u;
            }
        }
    }

    MSTResult* result = new MSTResult(v - 1);
    for (int i = 1; i < v; i++) {
        if (parent[i] != -1) {
            result->edges[result->edgeCount++] = {parent[i], i, key[i]};
            result->totalWeight += key[i];
        }
    }

    delete[] key;
    delete[] parent;
    delete[] inMST;

    return result;
}

MSTResult* primList(const AdjacencyList& graph) {
    int v = graph.getVertices();

    int* key = new int[v];
    int* parent = new int[v];
    bool* inMST = new bool[v];

    for (int i = 0; i < v; i++) {
        key[i] = -1;
        parent[i] = -1;
        inMST[i] = false;
    }

    key[0] = 0;

    for (int iter = 0; iter < v; iter++) {
        int u = minKey(key, inMST, v);
        if (u == -1) break;

        inMST[u] = true;

        ListNode* curr = graph.getHead(u);
        while (curr != nullptr) {
            int neighbor = curr->to;
            int w = curr->weight;

            if (!inMST[neighbor] && (key[neighbor] == -1 || w < key[neighbor])) {
                key[neighbor] = w;
                parent[neighbor] = u;
            }

            curr = curr->next;
        }
    }

    MSTResult* result = new MSTResult(v - 1);
    for (int i = 1; i < v; i++) {
        if (parent[i] != -1) {
            result->edges[result->edgeCount++] = {parent[i], i, key[i]};
            result->totalWeight += key[i];
        }
    }

    delete[] key;
    delete[] parent;
    delete[] inMST;

    return result;
}

#endif