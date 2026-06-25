#ifndef KRUSKALA_H
#define KRUSKALA_H

#include <iostream>
#include <type_traits>
#include "MSTResult.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "ShellSort.h"

// union-find (disjoint sets) with union by rank and path compression
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

    // find the set root with path compression
    int find(int x) {
        if (rodzic[x] != x) rodzic[x] = find(rodzic[x]);
        return rodzic[x];
    }

    // join two sets, returns false if they were already together
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

// edge used to collect and sort all edges of the graph
struct Krawedz {
    int from;
    int to;
    int waga;
};

// compare by weight so we can sort edges with the shellSort from P1
bool operator>(const Krawedz& a, const Krawedz& b) {
    return a.waga > b.waga;
}


// Kruskal's algorithm, works on both representations via templates
template <typename T>
MSTResult* kruskal(const T& graph) {
    int v = graph.getVertices();
    Krawedz* krawedzie = nullptr;
    int liczbaKrawedzi = 0;

    if constexpr (std::is_same<T, IncidenceMatrix>::value) {
        int e = graph.getEdges();
        krawedzie = new Krawedz[e];
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
    } else {
        for (int i = 0; i < v; i++) {
            ListNode* curr = graph.getHead(i);
            while (curr != nullptr) {
                if (curr->to > i) liczbaKrawedzi++;
                curr = curr->next;
            }
        }
        krawedzie = new Krawedz[liczbaKrawedzi];
        int idx = 0;
        for (int i = 0; i < v; i++) {
            ListNode* curr = graph.getHead(i);
            while (curr != nullptr) {
                if (curr->to > i) krawedzie[idx++] = {i, curr->to, curr->weight};
                curr = curr->next;
            }
        }
    }

    // bubble sort was slow for big graphs, used shellSort from P1 instead
    shellSort(krawedzie, liczbaKrawedzi, 1);
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