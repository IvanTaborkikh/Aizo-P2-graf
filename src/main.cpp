#include <iostream>
#include "Parameters.h"
#include "GraphFileReader.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "Prim.h"
#include "Kruskala.h"
#include "Dijkstry.h"
#include "FordaBellmana.h"

void runMST(const GraphData& data) {
    using namespace Parameters;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, false);
        im.print();
        std::cout << "\n";

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = primMatrix(im);
            result->print("Prim");
            delete result;
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = kruskalMatrix(im);
            result->print("Kruskala");
            delete result;
        }
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, false);
        al.print();
        std::cout << "\n";

        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = primList(al);
            result->print("Prim");
            delete result;
        }
        if (algorithm == Algorithms::kruskal || algorithm == Algorithms::allAlgorithms) {
            MSTResult* result = kruskalList(al);
            result->print("Kruskala");
            delete result;
        }
        std::cout << "\n";
    }
}

void runSP(const GraphData& data) {
    using namespace Parameters;

    int start = vertexStart;
    if (start == -1) start = 0;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, true);
        im.print();
        std::cout << "\n";

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = dijkstraMatrix(im, start);
            result->print("Dijkstry");
            delete result;
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = bellmanFordMatrix(im, start);
            printSP(result, "Forda-Bellmana");
            delete result;
        }
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, true);
        al.print();
        std::cout << "\n";

        if (algorithm == Algorithms::dijkstra || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = dijkstraList(al, start);
            result->print("Dijkstry");
            delete result;
        }
        if (algorithm == Algorithms::bellmanFord || algorithm == Algorithms::allAlgorithms) {
            SPResult* result = bellmanFordList(al, start);
            printSP(result, "Forda-Bellmana");
            delete result;
        }
        std::cout << "\n";
    }
}

void runSingleFile() {
    using namespace Parameters;

    if (inputFile.empty()) {
        std::cerr << "ERROR: No input file specified (use -i)" << std::endl;
        return;
    }

    GraphData data;
    if (!readGraphFromFile(inputFile, data)) {
        return;
    }

    std::cout << "Loaded graph: " << data.vertices << " vertices, "
              << data.edgeCount << " edges\n\n";

    if (problem == Problems::mst) {
        runMST(data);
    } else if (problem == Problems::sp) {
        runSP(data);
    } else {
        std::cerr << "ERROR: Problem not implemented yet" << std::endl;
    }
}

void runBenchmark() {
    std::cout << "Benchmark mode - not implemented yet" << std::endl;
}

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