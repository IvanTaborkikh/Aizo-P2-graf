#ifndef FORDABELLMANA_H
#define FORDABELLMANA_H

#include <iostream>
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "Dijkstry.h"

SPResult* bellmanFordMatrix(const IncidenceMatrix& graph, int start) {
    int v = graph.getVertices();
    int e = graph.getEdges();

    SPResult* result = new SPResult(v, start);
    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v - 1; iter++) {
        for (int j = 0; j < e; j++) {
            int from = -1, to = -1, waga = 0;

            for (int i = 0; i < v; i++) {
                int val = graph.getValue(i, j);
                if (val > 0) { from = i; waga = val; }
                else if (val < 0) { to = i; }
            }

            if (from == -1 || to == -1) continue;
            if (result->odleglosc[from] == -1) continue;

            int nowaOdleglosc = result->odleglosc[from] + waga;
            if (result->odleglosc[to] == -1 || nowaOdleglosc < result->odleglosc[to]) {
                result->odleglosc[to] = nowaOdleglosc;
                result->parent[to] = from;
            }
        }
    }

    return result;
}

SPResult* bellmanFordList(const AdjacencyList& graph, int start) {
    int v = graph.getVertices();

    SPResult* result = new SPResult(v, start);
    result->odleglosc[start] = 0;

    for (int iter = 0; iter < v - 1; iter++) {
        for (int i = 0; i < v; i++) {
            if (result->odleglosc[i] == -1) continue;

            ListNode* curr = graph.getHead(i);
            while (curr != nullptr) {
                int neighbor = curr->to;
                int waga = curr->weight;

                int nowaOdleglosc = result->odleglosc[i] + waga;
                if (result->odleglosc[neighbor] == -1 || nowaOdleglosc < result->odleglosc[neighbor]) {
                    result->odleglosc[neighbor] = nowaOdleglosc;
                    result->parent[neighbor] = i;
                }
                curr = curr->next;
            }
        }
    }

    return result;
}

void printSP(const SPResult* result, const std::string& algorithmName) {
    std::cout << "SP (" << algorithmName << ") from v" << result->start << ":\n";
    for (int i = 0; i < result->vertices; i++) {
        if (i == result->start) continue;
        if (result->odleglosc[i] == -1) {
            std::cout << "  v" << result->start << " -> v" << i << ": brak sciezki\n";
            continue;
        }

        int* sciezka = new int[result->vertices];
        int len = 0;
        int curr = i;
        while (curr != -1) {
            sciezka[len++] = curr;
            curr = result->parent[curr];
        }

        std::cout << "  v" << result->start;
        for (int j = len - 2; j >= 0; j--) {
            std::cout << " -> v" << sciezka[j];
        }
        std::cout << "  koszt: " << result->odleglosc[i] << "\n";
        delete[] sciezka;
    }
}

#endif