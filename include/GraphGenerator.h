#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "GraphFileReader.h"

// max possible number of edges for given vertex count
int maxEdgesForGraph(int v, bool directed) {
    if (directed) return v * (v - 1);
    return v * (v - 1) / 2;

}

// how many edges we need for given density (at least a spanning tree)
int targetEdgeCount(int v, int density, bool directed) {
    int minEdges = v - 1;
    int maxEdges = maxEdgesForGraph(v, directed);
    int target = (maxEdges) * density / 100;

    if (target < minEdges) target = minEdges;
    return target;
}

// generate a connected graph: first a random spanning tree, then extra edges
GraphData* generateGraph(int v, int density, bool directed) {
    // seed the random generator only once
    static bool wylosowano = false;
    if (!wylosowano) {
        srand(time(nullptr));
        wylosowano = true;
    }

    int doceloweLiczbaKrawedzi = targetEdgeCount(v, density, directed);

    int maxWaga = doceloweLiczbaKrawedzi * 4 / 5;
    if (maxWaga < 1) maxWaga = 1;

    GraphData* data = new GraphData();
    data->vertices = v;
    data->edges = new Edge[doceloweLiczbaKrawedzi];
    data->edgeCount = 0;


    int* kolejnosc = new int[v];
    bool* dodany = new bool[v];
    for (int i = 0; i < v; i++) {

        kolejnosc[i] = i;
        dodany[i] = false;
    }


    // random permutation of vertices (Fisher-Yates shuffle)
    for (int i = v - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = kolejnosc[i];
        kolejnosc[i] = kolejnosc[j];
        kolejnosc[j] = tmp;
    }


    // spanning tree: connect each next vertex to a random already-added one
    dodany[kolejnosc[0]] = true;
    for (int i = 1; i < v; i++) {
        int nowyWierzcholek = kolejnosc[i];
        int losowy = kolejnosc[rand() % i];
        int waga = rand() % maxWaga + 1;

        data->edges[data->edgeCount++] = {losowy, nowyWierzcholek, waga};
        dodany[nowyWierzcholek] = true;
    }


    int maxMozliwych = maxEdgesForGraph(v, directed);
    int* mozliweFrom = new int[maxMozliwych];
    int* mozliweTo = new int[maxMozliwych];
    int liczbaMozliwych = 0;


    bool** juzJest = new bool*[v];
    for (int i = 0; i < v; i++) {
        juzJest[i] = new bool[v];
        for (int j = 0; j < v; j++) juzJest[i][j] = false;
    }

    for (int i = 0; i < data->edgeCount; i++) {
        juzJest[data->edges[i].from][data->edges[i].to] = true;
        if (!directed) juzJest[data->edges[i].to][data->edges[i].from] = true;
    }

    // collect all edges that are not in the graph yet
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            if (i == j) continue;

            if (!directed && j < i) continue;
            if (juzJest[i][j]) continue;
            mozliweFrom[liczbaMozliwych] = i;
            mozliweTo[liczbaMozliwych] = j;
            liczbaMozliwych++;
        }
    }


    // shuffle the candidate edges
    for (int i = liczbaMozliwych - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmpF = mozliweFrom[i]; mozliweFrom[i] = mozliweFrom[j]; mozliweFrom[j] = tmpF;
        int tmpT = mozliweTo[i]; mozliweTo[i] = mozliweTo[j]; mozliweTo[j] = tmpT;
    }

    // add random candidate edges until we reach the target edge count
    int doDoania = doceloweLiczbaKrawedzi - data->edgeCount;
    for (int i = 0; i < doDoania && i < liczbaMozliwych; i++) {
        int waga = rand() % maxWaga + 1;
        data->edges[data->edgeCount++] = {mozliweFrom[i], mozliweTo[i], waga};
    }

    for (int i = 0; i < v; i++) delete[] juzJest[i];
    delete[] juzJest;
    delete[] mozliweFrom;
    delete[] mozliweTo;
    delete[] kolejnosc;
    delete[] dodany;

    return data;
}

// save graph to file in the same format as input files
void saveGraphToFile(const GraphData* data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot save graph to file" << std::endl;
        return;

    }
    file << data->vertices << "\t" << data->edgeCount << "\n";
    for (int i = 0; i < data->edgeCount; i++) {
        file << data->edges[i].from << "\t"
             << data->edges[i].to << "\t"
             << data->edges[i].weight << "\n";
    }
    file.close();
}

// print basic info about the generated graph (used for debugging)
void printGraphInfo(const GraphData* data, int density, bool directed) {
    int maxEdges = maxEdgesForGraph(data->vertices, directed);
    int expectedEdges = targetEdgeCount(data->vertices, density, directed);
    std::cout << "vertices: " << data->vertices << "\n";
    std::cout << "edges: " << data->edgeCount << " (expected: " << expectedEdges << ")\n";
    std::cout << "max possible edges: " << maxEdges << "\n";
    
}

#endif