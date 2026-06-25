#ifndef INCIDENCEMATRIX_H
#define INCIDENCEMATRIX_H

#include <iostream>
#include "GraphFileReader.h"

// graph stored as an incidence matrix (vertices x edges)
class IncidenceMatrix {
private:
    int** matrix;
    int vertices;
    int edges;

    // allocate matrix and fill it with zeros
    void allocate() {
        matrix = new int*[vertices];
        for (int i = 0; i < vertices; i++) {
            matrix[i] = new int[edges];
            for (int j = 0; j < edges; j++) {
                matrix[i][j] = 0;
            }

        }
    }

public:
    IncidenceMatrix(int v, int e) : vertices(v), edges(e) {
        allocate();
    }

    ~IncidenceMatrix() {
        for (int i = 0; i < vertices; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

    }

    // build matrix from edge list; for directed graph "from" gets +w and "to" gets -w
    void loadFromData(const GraphData& data, bool directed) {
        for (int i = 0; i < data.edgeCount; i++) {
            int from = data.edges[i].from;
            int to = data.edges[i].to;
            int w = data.edges[i].weight;

            if (directed) {
                matrix[from][i] = w;
                matrix[to][i] = -w;
            } else {
                matrix[from][i] = w;
                matrix[to][i] = w;
            }
        }
    }

    int getValue(int vertex, int edgeIndex) const {
        return matrix[vertex][edgeIndex];
    }


    int getVertices() const { return vertices; }
    int getEdges() const { return edges; }

    // verification: count columns (edges) that have any non-zero entry
    int policzKrawedzie() const {
        int licznik = 0;
        for (int j = 0; j < edges; j++) {
            bool pusta = true;
            for (int i = 0; i < vertices; i++) {
                if (matrix[i][j] != 0) { pusta = false; break; }
            }
            if (!pusta) licznik++;
        }
        return licznik;
    }

    // print the whole matrix to screen
    void print() const {
        std::cout << "Incidence matrix (" << vertices << "x" << edges << "):\n";
        std::cout << "     ";
        for (int j = 0; j < edges; j++) {
            std::cout << "e" << j << "  ";
        }
        std::cout << "\n";
        for (int i = 0; i < vertices; i++) {
            std::cout << "v" << i << "  ";
            for (int j = 0; j < edges; j++) {
                std::cout << " " << matrix[i][j] << "  ";
            }
            std::cout << "\n";
        }
        
    }
};

#endif