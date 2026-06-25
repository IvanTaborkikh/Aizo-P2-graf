#ifndef MFRESULT_H
#define MFRESULT_H

#include <iostream>

// result of max flow: flow value and source/sink vertices
struct MFResult {
    int maxFlow;
    int source;
    int sink;

    MFResult(int s, int t) : maxFlow(0), source(s), sink(t) {}

    // print source, sink and the max flow value
    void print() const {
        std::cout << "MF (Forda-Fulkersona):\n";
        std::cout << "  source: v" << source << ", sink: v" << sink << "\n";
        std::cout << "  max flow: " << maxFlow << "\n";
    }
    
};

#endif