#ifndef SPRESULT_H
#define SPRESULT_H

#include <iostream>
#include <string>

// use a big number as "infinity" instead of -1,
// because with negative weights a real distance can be -1 (clashes with the sentinel)
const int NIESKONCZONOSC = 1000000000;

// result of shortest path: distances and parents from the start vertex
struct SPResult {
    int* odleglosc;
    int* parent;
    int vertices;
    int start;
    bool ujemnyCykl;

    SPResult(int v, int s) : vertices(v), start(s), ujemnyCykl(false) {
        odleglosc = new int[v];
        parent = new int[v];
        for (int i = 0; i < v; i++) {
            odleglosc[i] = NIESKONCZONOSC;
            parent[i] = -1;
        }

    }

    ~SPResult() {
        delete[] odleglosc;
        delete[] parent;
    }

    // print path and cost to every reachable vertex
    void print(const std::string& algorithmName) const {
        std::cout << "SP (" << algorithmName << ") from v" << start << ":\n";
        if (ujemnyCykl) {
            // with a negative cycle the paths loop forever, so we don't rebuild them
            std::cout << "  UWAGA: wykryto ujemny cykl, wyniki moga byc niepoprawne!\n";
            return;
        }
        for (int i = 0; i < vertices; i++) {
            if (i == start) continue;

            if (odleglosc[i] == NIESKONCZONOSC) {
                std::cout << "  v" << start << " -> v" << i << ": brak sciezki\n";
                continue;
            }

            int* sciezka = new int[vertices];
            int len = 0;
            int curr = i;
            while (curr != -1 && len < vertices) {
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

#endif