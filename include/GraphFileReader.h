#ifndef GRAPHFILEREADER_H
#define GRAPHFILEREADER_H

#include <fstream>
#include <iostream>
#include <string>

struct Edge {
    int from;
    int to;
    int weight;
};

struct GraphData {
    int vertices;
    int edgeCount;
    Edge* edges;

    GraphData() : vertices(0), edgeCount(0), edges(nullptr) {}

    ~GraphData() {
        delete[] edges;
    }
};

inline bool readGraphFromFile(const std::string& filename, GraphData& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open file: " << filename << std::endl;
        return false;
    }

    file >> data.vertices >> data.edgeCount;
    if (data.vertices <= 0 || data.edgeCount <= 0) {
        std::cerr << "ERROR: Invalid graph size in file" << std::endl;
        return false;
    }

    data.edges = new Edge[data.edgeCount];

    for (int i = 0; i < data.edgeCount; i++) {
        file >> data.edges[i].from >> data.edges[i].to >> data.edges[i].weight;
    }

    if (file.fail()) {
        std::cerr << "ERROR: Failed to read edges from file" << std::endl;
        return false;
    }

    file.close();
    return true;
}

#endif