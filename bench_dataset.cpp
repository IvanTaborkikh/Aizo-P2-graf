// measure runtime of our shortest-path algorithms on the big dataset (bitcoin-alpha)
// results are appended to results/dataset_times.csv (grouped: iterations + avg/min/max)
// build:  g++ -std=c++17 -Wall -Wextra -Werror -Iinclude bench_dataset.cpp -o bench_dataset
// run (from project dir):  ./bench_dataset

#include "GraphFileReader.h"
#include "AdjacencyList.h"
#include "Dijkstry.h"
#include "FordaBellmana.h"
#include "CsvWriter.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

// append one row to the dataset times csv
void zapiszWiersz(const std::string& plik, const std::string& alg,
                  int v, int e, const std::string& iteracja, long long czas) {
    std::ofstream f(plik, std::ios::app);
    f << getCurrentDateTime() << "," << alg << ",lista," << v << "," << e
      << "," << iteracja << "," << czas << "\n";
}

int main() {
    const std::string inputFile = "data/bitcoin_alpha.txt";
    const std::string outFile = "results/dataset_times.csv";
    const int powtorzenia = 50;

    GraphData data;
    if (!readGraphFromFile(inputFile, data)) return 1;
    std::cout << "Dataset: " << data.vertices << " wierzcholkow, "
              << data.edgeCount << " krawedzi\n";

    // graf skierowany -> lista sasiedztwa (macierz dla tego rozmiaru bylaby za duza)
    AdjacencyList al(data.vertices);
    al.loadFromData(data, true);

    // header only if file does not exist yet
    std::ifstream check(outFile);
    bool exists = check.good();
    check.close();
    if (!exists) {
        std::ofstream f(outFile);
        f << "date,algorytm,struktura,wierzcholki,krawedzie,iteracja,czas_us\n";
    }

    // Dijkstra
    long long minC = -1, maxC = 0, suma = 0;
    for (int it = 0; it < powtorzenia; it++) {
        auto t1 = std::chrono::steady_clock::now();
        SPResult* r = dijkstra(al, 0);
        auto t2 = std::chrono::steady_clock::now();
        long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        delete r;
        zapiszWiersz(outFile, "dijkstra", data.vertices, data.edgeCount, std::to_string(it + 1), elapsed);
        suma += elapsed;
        if (minC < 0 || elapsed < minC) minC = elapsed;
        if (elapsed > maxC) maxC = elapsed;
    }
    zapiszWiersz(outFile, "dijkstra", data.vertices, data.edgeCount, "avg", suma / powtorzenia);
    zapiszWiersz(outFile, "dijkstra", data.vertices, data.edgeCount, "min", minC);
    zapiszWiersz(outFile, "dijkstra", data.vertices, data.edgeCount, "max", maxC);
    std::cout << "dijkstra (lista) -> avg: " << suma / powtorzenia
              << " us, min: " << minC << " us, max: " << maxC << " us\n";

    // Bellman-Ford
    minC = -1; maxC = 0; suma = 0;
    for (int it = 0; it < powtorzenia; it++) {
        auto t1 = std::chrono::steady_clock::now();
        SPResult* r = bellmanFord(al, 0);
        auto t2 = std::chrono::steady_clock::now();
        long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        delete r;
        zapiszWiersz(outFile, "bellman-ford", data.vertices, data.edgeCount, std::to_string(it + 1), elapsed);
        suma += elapsed;
        if (minC < 0 || elapsed < minC) minC = elapsed;
        if (elapsed > maxC) maxC = elapsed;
    }
    zapiszWiersz(outFile, "bellman-ford", data.vertices, data.edgeCount, "avg", suma / powtorzenia);
    zapiszWiersz(outFile, "bellman-ford", data.vertices, data.edgeCount, "min", minC);
    zapiszWiersz(outFile, "bellman-ford", data.vertices, data.edgeCount, "max", maxC);
    std::cout << "bellman-ford (lista) -> avg: " << suma / powtorzenia
              << " us, min: " << minC << " us, max: " << maxC << " us\n";

    std::cout << "Zapisano do " << outFile << "\n";
    return 0;
}
