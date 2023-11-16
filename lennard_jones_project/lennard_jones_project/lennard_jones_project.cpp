#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <mutex>
#include "jDE.h"
#include "utils.h"
#include "X.h"


// -N -seed -target [-nfesLmt | -runtimeLmt] -Np -expRuns -expThreads
int main(int argc, char* argv[])
{
    if (argc < 9) {
        std::cout << "Incorrect quantity of input parameters!\n";
        return 1;
    }


    // Parse command-line arguments
    unsigned int N, nfesLmt, runtimeLmt, Np, expThreads;
    double target;
    try {
        N = std::stoi(argv[2]);
        seedGl = std::stoi(argv[4]);
        target = std::stod(argv[6]);
        // Could be either nfesLmt or runtimeLmt
        if (std::string(argv[7]) == "-nfesLmt") {
            nfesLmt = std::stoi(argv[8]);
            runtimeLmt = -1;
        }
        else {
            nfesLmt = -1;
            runtimeLmt = std::stoi(argv[8]);
        }
        Np = std::stoi(argv[10]);
        expRuns = std::stoi(argv[12]);
        expThreads = std::stoi(argv[14]);
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return 1;
    }

    // Run expRuns amount of runs parallel across expThreads amount of threads
    std::vector <std::thread*> threadVec;
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < expThreads; i++) {
        std::thread* nit = new std::thread(startRuns, N, Np, nfesLmt, runtimeLmt, target);
        threadVec.push_back(nit);
    }

    // Wait for the thread to finish and delete it 
    // Should replace with smart pointers
    for (auto i : threadVec) {
        i->join();
        delete i;
    }

    // Measure and output the entire runtime for expRuns solutions
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    std::cout << "Entire duration: " << duration << "\n";
}