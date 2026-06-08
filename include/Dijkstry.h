#ifndef DIJKSTRY_H
#define DIJKSTRY_H

#include <iostream>
#include <string>
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"

struct SPResult {
    int* odleglosc;
    int* parent;
    int vertices;
    int start;

    SPResult(int v, int s) : vertices(v), start(s) {
        odleglosc = new int[v];
        parent = new int[v];
        for (int i = 0; i < v; i++) {
            odleglosc[i] = -1;
            parent[i] = -1;
        }
    }

    ~SPResult() {
        delete[] odleglosc;
        delete[] parent;
    }

    void print(const std::string& algorithmName = "Dijkstry") const {
        std::cout << "SP (" << algorithmName << ") from v" << start << ":\n";
        for (int i = 0; i < vertices; i++) {
            if (i == start) continue;
            if (odleglosc[i] == -1) {
                std::cout << "  v" << start << " -> v" << i << ": brak sciezki\n";
                continue;
            }

            int* sciezka = new int[vertices];
            int len = 0;
            int curr = i;
            while (curr != -1) {
                sciezka[len++] = curr;
                curr = parent[curr];
            }

            std::cout << "  v" << start;
            for (int j = len - 2; j >= 0; j--) {
                std::cout << " -> v" << sciezka[j];
            }
            std::cout << "  koszt: " << odleglosc[i] << "\n";
            delete[] sciezka;
        }
    }
};

int minOdleglosc(int* odleglosc, bool* odwiedzone, int v) {
    int min = -1;
    for (int i = 0; i < v; i++) {
        if (!odwiedzone[i] && odleglosc[i] != -1) {
            if (min == -1 || odleglosc[i] < odleglosc[min]) {
                min = i;
            }
        }
    }
    return min;
}

SPResult* dijkstraMatrix(const IncidenceMatrix& graph, int start) {
    int v = graph.getVertices();
    int e = graph.getEdges();

    SPResult* result = new SPResult(v, start);
    bool* odwiedzone = new bool[v];
    for (int i = 0; i < v; i++) odwiedzone[i] = false;

    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v; iter++) {
        int u = minOdleglosc(result->odleglosc, odwiedzone, v);
        if (u == -1) break;
        odwiedzone[u] = true;

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
            if (result->odleglosc[neighbor] == -1 || nowaOdleglosc < result->odleglosc[neighbor]) {
                result->odleglosc[neighbor] = nowaOdleglosc;
                result->parent[neighbor] = u;
            }
        }
    }

    delete[] odwiedzone;
    return result;
}

SPResult* dijkstraList(const AdjacencyList& graph, int start) {
    int v = graph.getVertices();

    SPResult* result = new SPResult(v, start);
    bool* odwiedzone = new bool[v];
    for (int i = 0; i < v; i++) odwiedzone[i] = false;

    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v; iter++) {
        int u = minOdleglosc(result->odleglosc, odwiedzone, v);
        if (u == -1) break;
        odwiedzone[u] = true;

        ListNode* curr = graph.getHead(u);
        while (curr != nullptr) {
            int neighbor = curr->to;
            int waga = curr->weight;

            if (!odwiedzone[neighbor]) {
                int nowaOdleglosc = result->odleglosc[u] + waga;
                if (result->odleglosc[neighbor] == -1 || nowaOdleglosc < result->odleglosc[neighbor]) {
                    result->odleglosc[neighbor] = nowaOdleglosc;
                    result->parent[neighbor] = u;
                }
            }
            curr = curr->next;
        }
    }

    delete[] odwiedzone;
    return result;
}

#endif