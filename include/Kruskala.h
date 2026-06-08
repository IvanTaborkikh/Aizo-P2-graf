#ifndef KRUSKALA_H
#define KRUSKALA_H

#include <iostream>
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "Prim.h"

struct UnionFind {
    int* rodzic;
    int* rank;
    int rozmiar;

    UnionFind(int n) : rozmiar(n) {
        rodzic = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            rodzic[i] = i;
            rank[i] = 0;
        }
    }

    ~UnionFind() {
        delete[] rodzic;
        delete[] rank;
    }

    int find(int x) {
        if (rodzic[x] != x) rodzic[x] = find(rodzic[x]);
        return rodzic[x];
    }

    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;

        if (rank[px] < rank[py]) rodzic[px] = py;
        else if (rank[px] > rank[py]) rodzic[py] = px;
        else { rodzic[py] = px; rank[px]++; }

        return true;
    }
};

struct Krawedz {
    int from;
    int to;
    int waga;
};

void sortujKrawedzie(Krawedz* krawedzie, int liczba) {
    for (int i = 0; i < liczba - 1; i++) {
        for (int j = 0; j < liczba - i - 1; j++) {
            if (krawedzie[j].waga > krawedzie[j + 1].waga) {
                Krawedz tmp = krawedzie[j];
                krawedzie[j] = krawedzie[j + 1];
                krawedzie[j + 1] = tmp;
            }
        }
    }
}

MSTResult* kruskalMatrix(const IncidenceMatrix& graph) {
    int v = graph.getVertices();
    int e = graph.getEdges();

    Krawedz* krawedzie = new Krawedz[e];
    int liczbaKrawedzi = 0;

    for (int j = 0; j < e; j++) {
        int from = -1, to = -1, waga = 0;
        for (int i = 0; i < v; i++) {
            int val = graph.getValue(i, j);
            if (val > 0) {
                if (from == -1) { from = i; waga = val; }
                else to = i;
            } else if (val < 0) {
                to = i;
            }
        }
        if (from != -1 && to != -1) krawedzie[liczbaKrawedzi++] = {from, to, waga};
    }

    sortujKrawedzie(krawedzie, liczbaKrawedzi);

    UnionFind uf(v);
    MSTResult* result = new MSTResult(v - 1);

    for (int i = 0; i < liczbaKrawedzi; i++) {
        if (uf.unite(krawedzie[i].from, krawedzie[i].to)) {
            result->edges[result->edgeCount++] = {krawedzie[i].from, krawedzie[i].to, krawedzie[i].waga};
            result->totalWeight += krawedzie[i].waga;
        }
        if (result->edgeCount == v - 1) break;
    }

    delete[] krawedzie;
    return result;
}

MSTResult* kruskalList(const AdjacencyList& graph) {
    int v = graph.getVertices();

    int liczbaKrawedzi = 0;
    for (int i = 0; i < v; i++) {
        ListNode* curr = graph.getHead(i);
        while (curr != nullptr) {
            if (curr->to > i) liczbaKrawedzi++;
            curr = curr->next;
        }
    }

    Krawedz* krawedzie = new Krawedz[liczbaKrawedzi];
    int idx = 0;

    for (int i = 0; i < v; i++) {
        ListNode* curr = graph.getHead(i);
        while (curr != nullptr) {
            if (curr->to > i) krawedzie[idx++] = {i, curr->to, curr->weight};
            curr = curr->next;
        }
    }

    sortujKrawedzie(krawedzie, liczbaKrawedzi);

    UnionFind uf(v);
    MSTResult* result = new MSTResult(v - 1);

    for (int i = 0; i < liczbaKrawedzi; i++) {
        if (uf.unite(krawedzie[i].from, krawedzie[i].to)) {
            result->edges[result->edgeCount++] = {krawedzie[i].from, krawedzie[i].to, krawedzie[i].waga};
            result->totalWeight += krawedzie[i].waga;
        }
        if (result->edgeCount == v - 1) break;
    }

    delete[] krawedzie;
    return result;
}

#endif