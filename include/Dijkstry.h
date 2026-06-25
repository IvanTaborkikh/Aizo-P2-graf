#ifndef DIJKSTRY_H
#define DIJKSTRY_H

#include <iostream>
#include <type_traits>
#include "SPResult.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

// find the unvisited vertex with the smallest distance
int minOdleglosc(int* odleglosc, bool* odwiedzone, int v) {
    int min = -1;
    for (int i = 0; i < v; i++) {
        if (!odwiedzone[i] && odleglosc[i] != NIESKONCZONOSC) {
            if (min == -1 || odleglosc[i] < odleglosc[min]) {
                min = i;
            }
            
        }
    }
    return min;
}

// Dijkstra's algorithm (positive weights), works on both representations
template <typename T>
SPResult* dijkstra(const T& graph, int start) {
    int v = graph.getVertices();

    SPResult* result = new SPResult(v, start);
    bool* odwiedzone = new bool[v];

    for (int i = 0; i < v; i++) odwiedzone[i] = false;

    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v; iter++) {
        int u = minOdleglosc(result->odleglosc, odwiedzone, v);
        if (u == -1) break;
        odwiedzone[u] = true;

        if constexpr (std::is_same<T, IncidenceMatrix>::value) {
            int e = graph.getEdges();
            for (int j = 0; j < e; j++) {
                if (graph.getValue(u, j) <= 0) continue;
                int waga = graph.getValue(u, j);
                int neighbor = -1;
                for (int i = 0; i < v; i++) {
                    if (i != u && graph.getValue(i, j) < 0) {
                        neighbor = i;
                        break;
                    }

                }
                if (neighbor == -1 || odwiedzone[neighbor]) continue;
                int nowaOdleglosc = result->odleglosc[u] + waga;
                if (result->odleglosc[neighbor] == NIESKONCZONOSC || nowaOdleglosc < result->odleglosc[neighbor]) {
                    result->odleglosc[neighbor] = nowaOdleglosc;
                    result->parent[neighbor] = u;
                }
            }
        } else {
            ListNode* curr = graph.getHead(u);
            while (curr != nullptr) {
                int neighbor = curr->to;
                int waga = curr->weight;
                if (!odwiedzone[neighbor]) {
                    int nowaOdleglosc = result->odleglosc[u] + waga;
                    if (result->odleglosc[neighbor] == NIESKONCZONOSC || nowaOdleglosc < result->odleglosc[neighbor]) {
                        result->odleglosc[neighbor] = nowaOdleglosc;
                        result->parent[neighbor] = u;

                    }
                }
                curr = curr->next;
            }
        }
    }


    delete[] odwiedzone;
    return result;
}

#endif