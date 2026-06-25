#include <iostream>
#include <chrono>
#include <string>
#include "Parameters.h"
#include "GraphFileReader.h"
#include "GraphGenerator.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "MSTResult.h"
#include "SPResult.h"
#include "MFResult.h"
#include "Prim.h"
#include "Kruskala.h"
#include "Dijkstry.h"
#include "FordaBellmana.h"
#include "FordaFulkersona.h"
#include "CsvWriter.h"

// check if edge count in the loaded structure matches the input file
void weryfikujKrawedzie(int policzone, int oczekiwane) {
    if (policzone == oczekiwane) {
        std::cout << "Weryfikacja: liczba krawedzi w strukturze zgadza sie z plikiem ("
                  << policzone << ")\n";
    } else {
        std::cout << "Weryfikacja: UWAGA! krawedzi w strukturze: " << policzone
                  << ", a w pliku: " << oczekiwane << "\n";
    }
}

// single test: solve MST on selected representations (graph is undirected)
void runMST(const GraphData& data) {
    using namespace Parameters;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, false);
        im.print();
        weryfikujKrawedzie(im.policzKrawedzie(), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = prim(im);
            result->print("Prim");
            delete result;
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = kruskal(im);
            result->print("Kruskala");
            delete result;
        }
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, false);
        al.print();
        weryfikujKrawedzie(al.policzKrawedzie(false), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = prim(al);
            result->print("Prim");
            delete result;
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = kruskal(al);
            result->print("Kruskala");
            delete result;
        }
        std::cout << "\n";
    }
}

// single test: solve shortest path (graph is directed), default start vertex is 0
void runSP(const GraphData& data) {
    using namespace Parameters;

    int start = vertexStart;
    if (start == -1) start = 0;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, true);
        im.print();
        weryfikujKrawedzie(im.policzKrawedzie(), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = dijkstra(im, start);
            result->print("Dijkstry");
            delete result;
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = bellmanFord(im, start);
            result->print("Forda-Bellmana");
            delete result;
        }
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, true);
        al.print();
        weryfikujKrawedzie(al.policzKrawedzie(true), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = dijkstra(al, start);
            result->print("Dijkstry");
            delete result;
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = bellmanFord(al, start);
            result->print("Forda-Bellmana");
            delete result;
        }
        std::cout << "\n";
    }
}

// single test: solve max flow (graph is directed), default source 0 and sink = last vertex
void runMF(const GraphData& data) {
    using namespace Parameters;

    int source = vertexStart;
    int sink = vertexEnd;
    if (source == -1) source = 0;
    if (sink == -1) sink = data.vertices - 1;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, true);
        im.print();
        weryfikujKrawedzie(im.policzKrawedzie(), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::fordFulkerson || algorithm == Algorithms::allAlgorithms) {
            MFResult* result = fordFulkerson(im, source, sink);
            result->print();
            delete result;
        }
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, true);
        al.print();
        weryfikujKrawedzie(al.policzKrawedzie(true), data.edgeCount);
        std::cout << "\n";

        if (algorithm == Algorithms::fordFulkerson || algorithm == Algorithms::allAlgorithms) {
            MFResult* result = fordFulkerson(al, source, sink);
            result->print();
            delete result;
        }
        std::cout << "\n";
    }
}

// load graph from file and solve the selected problem
void runSingleFile() {
    using namespace Parameters;

    if (inputFile.empty()) {
        std::cerr << "ERROR: No input file" << std::endl;
        return;
    }

    GraphData data;
    if (!readGraphFromFile(inputFile, data)) {
        return;
    }

    // if output file is given, send all printing there (graph + solution)
    std::ofstream out;
    std::streambuf* staryBufor = nullptr;
    if (!outputFile.empty()) {
        out.open(outputFile);
        if (out.is_open()) {
            staryBufor = std::cout.rdbuf(out.rdbuf());
        } else {
            std::cerr << "ERROR: Cannot open output file: " << outputFile << std::endl;
        }
    }

    std::cout << "Read graph: " << data.vertices << " vertices, "
              << data.edgeCount << " edges from " << inputFile << "\n\n";

    if (problem == Problems::mst) {
        runMST(data);
    } else if (problem == Problems::sp) {
        runSP(data);
    } else if (problem == Problems::mf) {
        runMF(data);
    } else {
        std::cerr << "ERROR: No problem specified (use -p)" << std::endl;
    }

    // restore normal screen output
    if (staryBufor != nullptr) {
        std::cout.rdbuf(staryBufor);
        std::cout << "Saved to " << outputFile << std::endl;
    }
}

// statistics and per-iteration times for one problem/algorithm/structure combination
struct Pomiar {
    std::string problem;
    std::string algorytm;
    std::string struktura;
    long long minCzas;
    long long maxCzas;
    long long sumaCzas;
    int liczba;
    long long* czasy;   // time of each iteration, written grouped at the end
};

// store the time of iteration "it" for the matching combination, create it if needed
void zapiszWynik(Pomiar* pomiary, int& liczbaPomiarow, int iterations, int it,
                 const std::string& problem, const std::string& algorytm,
                 const std::string& struktura, long long czas) {
    for (int i = 0; i < liczbaPomiarow; i++) {
        if (pomiary[i].problem == problem && pomiary[i].algorytm == algorytm
            && pomiary[i].struktura == struktura) {
            pomiary[i].czasy[it] = czas;
            if (czas < pomiary[i].minCzas) pomiary[i].minCzas = czas;
            if (czas > pomiary[i].maxCzas) pomiary[i].maxCzas = czas;
            pomiary[i].sumaCzas += czas;
            pomiary[i].liczba++;
            return;
        }
    }
    pomiary[liczbaPomiarow] = {problem, algorytm, struktura, czas, czas, czas, 1,
                              new long long[iterations]};
    pomiary[liczbaPomiarow].czasy[it] = czas;
    liczbaPomiarow++;
}

// benchmark MST: measure only the algorithm, loading is done before the timer
void benchmarkMST(GraphData* data, int it, Pomiar* pomiary, int& liczbaPomiarow) {
    using namespace Parameters;

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data->vertices);
        al.loadFromData(*data, false);

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MSTResult* r = prim(al);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mst", "prim", "lista", elapsed);
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MSTResult* r = kruskal(al);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mst", "kruskal", "lista", elapsed);
        }
    }

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data->vertices, data->edgeCount);
        im.loadFromData(*data, false);

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MSTResult* r = prim(im);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mst", "prim", "macierz", elapsed);
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MSTResult* r = kruskal(im);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mst", "kruskal", "macierz", elapsed);
        }
    }
}

// benchmark SP: measure only the algorithm, loading is done before the timer
void benchmarkSP(GraphData* data, int it, Pomiar* pomiary, int& liczbaPomiarow) {
    using namespace Parameters;

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data->vertices);
        al.loadFromData(*data, true);

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            SPResult* r = dijkstra(al, 0);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "sp", "dijkstra", "lista", elapsed);
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            SPResult* r = bellmanFord(al, 0);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "sp", "bellman-ford", "lista", elapsed);
        }
    }

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data->vertices, data->edgeCount);
        im.loadFromData(*data, true);

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            SPResult* r = dijkstra(im, 0);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "sp", "dijkstra", "macierz", elapsed);
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            SPResult* r = bellmanFord(im, 0);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "sp", "bellman-ford", "macierz", elapsed);
        }
    }
}

// benchmark MF: measure only the algorithm, loading is done before the timer
void benchmarkMF(GraphData* data, int it, Pomiar* pomiary, int& liczbaPomiarow) {
    using namespace Parameters;

    int sink = data->vertices - 1;

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data->vertices);
        al.loadFromData(*data, true);

        if (algorithm == Algorithms::fordFulkerson || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MFResult* r = fordFulkerson(al, 0, sink);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mf", "ford-fulkerson", "lista", elapsed);
        }
    }

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data->vertices, data->edgeCount);
        im.loadFromData(*data, true);

        if (algorithm == Algorithms::fordFulkerson || algorithm == Algorithms::allAlgorithms) {
            auto t1 = std::chrono::steady_clock::now();
            MFResult* r = fordFulkerson(im, 0, sink);
            auto t2 = std::chrono::steady_clock::now();
            long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            delete r;
            zapiszWynik(pomiary, liczbaPomiarow, iterations, it, "mf", "ford-fulkerson", "macierz", elapsed);
        }
    }
}

// benchmark mode: generate graphs, run algorithms many times, save times and stats
void runBenchmark() {
    using namespace Parameters;

    if (vertexCount <= 0) {
        std::cerr << "ERROR: vertexCount must be > 0 (use -l)" << std::endl;
        return;
    }
    if (iterations <= 0) {
        std::cerr << "ERROR: iterations must be > 0 (use -n)" << std::endl;
        return;
    }
    if (resultsFile.empty()) {
        std::cerr << "ERROR: No results file (use -r)" << std::endl;
        return;
    }

    bool directed = (problem == Problems::sp || problem == Problems::mf);

    std::cout << "Running benchmark: "
              << iterations << " iterations, "
              << vertexCount << " vertices, "
              << density << "% density" << std::endl;

    writeCsvHeaderIfNeeded(resultsFile);

    // array for statistics (min/max/sum) per algorithm+structure combination
    Pomiar* pomiary = new Pomiar[20];
    int liczbaPomiarow = 0;

    for (int it = 0; it < iterations; it++) {
        GraphData* data = generateGraph(vertexCount, density, directed);
        // printGraphInfo(data, density, directed);
        // saveGraphToFile(data, "../generated.txt");

        if (problem == Problems::mst) {
            benchmarkMST(data, it, pomiary, liczbaPomiarow);
        } else if (problem == Problems::sp) {
            benchmarkSP(data, it, pomiary, liczbaPomiarow);
        } else if (problem == Problems::mf) {
            benchmarkMF(data, it, pomiary, liczbaPomiarow);
        } else {
            benchmarkMST(data, it, pomiary, liczbaPomiarow);
            benchmarkSP(data, it, pomiary, liczbaPomiarow);
        }

        delete data;
        std::cout << "it " << it + 1 << ": done" << std::endl;
    }

    // write results grouped per combination: all iterations, then avg/min/max
    for (int i = 0; i < liczbaPomiarow; i++) {
        Pomiar& p = pomiary[i];
        for (int j = 0; j < p.liczba; j++) {
            appendCsvRow(resultsFile, p.problem, p.algorytm, p.struktura,
                         vertexCount, density, std::to_string(j + 1), p.czasy[j]);
        }
        long long avg = p.sumaCzas / p.liczba;
        appendCsvRow(resultsFile, p.problem, p.algorytm, p.struktura,
                     vertexCount, density, "avg", avg);
        appendCsvRow(resultsFile, p.problem, p.algorytm, p.struktura,
                     vertexCount, density, "min", p.minCzas);
        appendCsvRow(resultsFile, p.problem, p.algorytm, p.struktura,
                     vertexCount, density, "max", p.maxCzas);

        std::cout << p.problem << "/" << p.algorytm << "/" << p.struktura
                  << " -> min: " << p.minCzas << " us, max: " << p.maxCzas
                  << " us, avg: " << avg << " us" << std::endl;
        delete[] p.czasy;
    }

    delete[] pomiary;
    std::cout << "Results saved to " << resultsFile << std::endl;
}

// read parameters and run the selected mode (single file / benchmark / help)
int main(int argc, char** argv) {
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        std::cerr << "Failed to read parameters!" << std::endl;
        return 1;
    }

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            break;
        case Parameters::RunModes::singleFile:
            runSingleFile();
            break;
        case Parameters::RunModes::benchmark:
            runBenchmark();
            break;
        default:
            std::cerr << "ERROR: No mode specified. Use --help for usage." << std::endl;
            return 1;
    }

    return 0;
}