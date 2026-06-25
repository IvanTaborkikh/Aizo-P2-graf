#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

// get current date and time as formatted string
inline std::string getCurrentDateTime() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buf);
}

// write csv header only if file doesn't exist yet
inline void writeCsvHeaderIfNeeded(const std::string& filename) {
    std::ifstream check(filename);
    bool exists = check.good();
    check.close();


    if (!exists) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "ERROR: Could not create results file: " << filename << std::endl;
            return;
        }
        file << "date,problem,algorytm,struktura,wierzcholki,gestosc,iteracja,czas_us\n";
        file.close();
    }
}

// append one row with benchmark results to csv file
inline void appendCsvRow(const std::string& filename,
                         const std::string& problem,
                         const std::string& algorytm,
                         const std::string& struktura,
                         int wierzcholki,
                         int gestosc,
                         const std::string& iteracja,
                         long long czas) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open results file: " << filename << std::endl;
        return;
        
    }

    file << getCurrentDateTime() << ","
         << problem << ","
         << algorytm << ","
         << struktura << ","
         << wierzcholki << ","
         << gestosc << ","
         << iteracja << ","
         << czas << "\n";

    file.close();
}

#endif