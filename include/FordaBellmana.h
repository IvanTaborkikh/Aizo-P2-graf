#ifndef FORDABELLMANA_H
#define FORDABELLMANA_H

#include <iostream>
#include <type_traits>
#include "SPResult.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

// Bellman-Ford: relax all edges v-1 times, works on both representations
template <typename T>
SPResult* bellmanFord(const T& graph, int start) {
    int v = graph.getVertices();

    SPResult* result = new SPResult(v, start);
    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v - 1; iter++) {
        if constexpr (std::is_same<T, IncidenceMatrix>::value) {
            int e = graph.getEdges();
            for (int j = 0; j < e; j++) {
                
                int from = -1, to = -1, waga = 0;
                for (int i = 0; i < v; i++) {
                    int val = graph.getValue(i, j);
                    if (val > 0) { from = i; waga = val; }
                    else if (val < 0) { to = i; }
                }
                if (from == -1 || to == -1) continue;

                if (result->odleglosc[from] == NIESKONCZONOSC) continue;
                int nowaOdleglosc = result->odleglosc[from] + waga;
                if (result->odleglosc[to] == NIESKONCZONOSC || nowaOdleglosc < result->odleglosc[to]) {
                    result->odleglosc[to] = nowaOdleglosc;
                    result->parent[to] = from;
                }
            }
        } else {
            for (int i = 0; i < v; i++) {
                if (result->odleglosc[i] == NIESKONCZONOSC) continue;
                ListNode* curr = graph.getHead(i);
                while (curr != nullptr) {
                    int neighbor = curr->to;
                    int waga = curr->weight;
                    int nowaOdleglosc = result->odleglosc[i] + waga;
                    if (result->odleglosc[neighbor] == NIESKONCZONOSC || nowaOdleglosc < result->odleglosc[neighbor]) {
                        result->odleglosc[neighbor] = nowaOdleglosc;
                        result->parent[neighbor] = i;
                    }
                    curr = curr->next;
                }
            }

        }
    }

    // one more pass: if any edge can still be relaxed, there is a negative cycle
    if constexpr (std::is_same<T, IncidenceMatrix>::value) {
        int e = graph.getEdges();
        for (int j = 0; j < e; j++) {
            int from = -1, to = -1, waga = 0;
            for (int i = 0; i < v; i++) {
                int val = graph.getValue(i, j);
                if (val > 0) { from = i; waga = val; }

                else if (val < 0) { to = i; }
            }
            if (from == -1 || to == -1) continue;
            if (result->odleglosc[from] == NIESKONCZONOSC || result->odleglosc[to] == NIESKONCZONOSC) continue;
            if (result->odleglosc[from] + waga < result->odleglosc[to]) {
                result->ujemnyCykl = true;
            }
        }
    } else {
        for (int i = 0; i < v; i++) {
            if (result->odleglosc[i] == NIESKONCZONOSC) continue;
            ListNode* curr = graph.getHead(i);
            while (curr != nullptr) {
                int neighbor = curr->to;
                int waga = curr->weight;
                if (result->odleglosc[neighbor] != NIESKONCZONOSC &&
                    result->odleglosc[i] + waga < result->odleglosc[neighbor]) {
                    result->ujemnyCykl = true;
                }
                curr = curr->next;
            }

        }
    }

    return result;
}

#endif