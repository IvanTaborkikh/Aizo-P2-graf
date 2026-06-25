#ifndef MSTRESULT_H
#define MSTRESULT_H

#include <iostream>
#include <string>

// single edge of the resulting spanning tree
struct MSTEdge {
    int from;
    int to;
    int weight;
};

// result of MST: list of tree edges and their total weight
struct MSTResult {
    MSTEdge* edges;
    int edgeCount;
    int totalWeight;

    MSTResult(int maxEdges) : edgeCount(0), totalWeight(0) {
        edges = new MSTEdge[maxEdges];
    }

    ~MSTResult() {
        delete[] edges;
    }

    // print the tree edges and the total weight
    void print(const std::string& algorithmName) const {
        std::cout << "MST (" << algorithmName << "):\n";
        for (int i = 0; i < edgeCount; i++) {
            std::cout << "  " << edges[i].from << " -- " << edges[i].to
                      << "  weight: " << edges[i].weight << "\n";
        }
        std::cout << "Total weight: " << totalWeight << "\n";
    }
    
};

#endif