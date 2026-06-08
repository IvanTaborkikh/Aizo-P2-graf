#include <iostream>
#include "Parameters.h"
#include "GraphFileReader.h"
#include "IncidenceMatrix.h"
#include "AdjacencyList.h"
#include "Prim.h"

void runMST(const GraphData& data) {
    using namespace Parameters;

    if (structure == Structures::incidenceMatrix || structure == Structures::allStructures) {
        IncidenceMatrix im(data.vertices, data.edgeCount);
        im.loadFromData(data, false);
        im.print();
        std::cout << "\n";

        MSTResult* result = primMatrix(im);
        result->print();
        delete result;
        std::cout << "\n";
    }

    if (structure == Structures::adjacencyList || structure == Structures::allStructures) {
        AdjacencyList al(data.vertices);
        al.loadFromData(data, false);
        al.print();
        std::cout << "\n";

        MSTResult* result = primList(al);
        result->print();
        delete result;
        std::cout << "\n";
    }
}

void runSingleFile() {
    using namespace Parameters;

    if (inputFile.empty()) {
        std::cerr << "No file" << std::endl;
        return;
    }

    GraphData data;
    if (!readGraphFromFile(inputFile, data)) {
        return;
    }


    if (problem == Problems::mst) {
        if (algorithm == Algorithms::prim || algorithm == Algorithms::allAlgorithms) {
            runMST(data);
        } else {
            std::cerr << "No algo" << std::endl;
        }
    } else {
        std::cerr << "No probl" << std::endl;
    }
}

void runBenchmark() {
    std::cout << "wait" << std::endl;
}

int main(int argc, char** argv) {
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        std::cerr << "No param" << std::endl;
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
            std::cerr << "No spec" << std::endl;
            return 1;
    }

    return 0;
}