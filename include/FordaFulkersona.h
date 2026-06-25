#ifndef FORDAFULKERSONA_H
#define FORDAFULKERSONA_H

#include <iostream>
#include <type_traits>
#include "MFResult.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

// BFS that finds an augmenting path in the residual network
bool bfs(int** residual, int v, int source, int sink, int* parent) {
    bool* odwiedzone = new bool[v];
    for (int i = 0; i < v; i++) {
        odwiedzone[i] = false;
        parent[i] = -1;

    }

    int* kolejka = new int[v];
    int poczatek = 0, koniec = 0;

    kolejka[koniec++] = source;
    odwiedzone[source] = true;

    while (poczatek < koniec) {
        int u = kolejka[poczatek++];
        for (int next = 0; next < v; next++) {
            if (!odwiedzone[next] && residual[u][next] > 0) {
                kolejka[koniec++] = next;
                odwiedzone[next] = true;
                parent[next] = u;
            }
        }

    }

    bool found = odwiedzone[sink];
    delete[] odwiedzone;
    delete[] kolejka;
    return found;
}

// Ford-Fulkerson max flow using BFS to find paths (Edmonds-Karp style)
template <typename T>
MFResult* fordFulkerson(const T& graph, int source, int sink) {
    int v = graph.getVertices();

    // residual capacity matrix, built from the graph below
    int** residual = new int*[v];
    for (int i = 0; i < v; i++) {
        residual[i] = new int[v];

        for (int j = 0; j < v; j++) residual[i][j] = 0;
    }

    if constexpr (std::is_same<T, IncidenceMatrix>::value) {
        int e = graph.getEdges();
        for (int j = 0; j < e; j++) {
            int from = -1, to = -1, waga = 0;
            for (int i = 0; i < v; i++) {
                int val = graph.getValue(i, j);
                if (val > 0) { from = i; waga = val; }
                else if (val < 0) { to = i; }
            }
            if (from != -1 && to != -1) residual[from][to] += waga;
        }
    } else {
        for (int i = 0; i < v; i++) {
            ListNode* curr = graph.getHead(i);
            while (curr != nullptr) {
                residual[i][curr->to] += curr->weight;
                curr = curr->next;
            }

        }
    }

    MFResult* result = new MFResult(source, sink);
    int* parent = new int[v];

    // keep finding augmenting paths and push the bottleneck flow along them
    while (bfs(residual, v, source, sink, parent)) {
        int pathFlow = residual[parent[sink]][sink];
        int curr = sink;
        while (curr != source) {
            int prev = parent[curr];
            if (residual[prev][curr] < pathFlow) pathFlow = residual[prev][curr];
            curr = prev;
        }

        curr = sink;
        while (curr != source) {
            int prev = parent[curr];
            residual[prev][curr] -= pathFlow;
            residual[curr][prev] += pathFlow;
            curr = prev;
            
        }

        result->maxFlow += pathFlow;
    }

    for (int i = 0; i < v; i++) delete[] residual[i];
    delete[] residual;
    delete[] parent;

    return result;
}

#endif