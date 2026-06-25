#ifndef PRIM_H
#define PRIM_H

#include <iostream>
#include <type_traits>
#include "MSTResult.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

// find the vertex not yet in the tree with the smallest key
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

// Prim's algorithm, works on both representations via templates
template <typename T>
MSTResult* prim(const T& graph) {
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

        // incidence matrix: look through all edges incident to u
        if constexpr (std::is_same<T, IncidenceMatrix>::value) {
            int e = graph.getEdges();
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
        } else {
            // adjacency list: go through the neighbours of u
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
    }

    // build the result tree from the parent[] array
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