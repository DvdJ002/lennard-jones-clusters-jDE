#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include "jDE.h"
#include "utils.h"
#include "X.h"


// -N -seed -target [-nfesLmt | -runtimeLmt] -Np
int main(int argc, char* argv[])
{
    if (argc < 9) {
        std::cout << "Incorrect quantity of input parameters!\n";
        return 1;
    }

    // Parse command-line arguments
    unsigned int N, seed, nfesLmt, runtimeLmt, Np;
    double target;
    try {
        N = std::stoi(argv[2]);
        seed = std::stoi(argv[4]);
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
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return 1;
    }

    // Algorithm setup
    std::mt19937 generator(seed);
    std::vector<X> population;
    unsigned int nfes = 0;

    // Initialize a population and insert random doubles in each X
    for (int i = 0; i < Np; ++i) {
        population.emplace_back(N);
    }
    for (auto& item : population) {
        for (int i = 0; i < item.coords.size(); i++) {
            item.coords[i] = getRandomDouble(generator) * (XjU - XjL) + XjL;
        }
    }

    // First best X, to be replaced with a better X
    X best(N);
    for (double& coord : best.coords) {
        coord = getRandomDouble(generator) * (XjU - XjL) + XjL;
    }

    // Start execution of jDE algorithm
    auto runtime =
        startAlgorithm(best, population, N, Np, nfes, nfesLmt, runtimeLmt, generator, target);

    // Output various algorithm results to console
    std::cout << "N: " << N << "\n";
    std::cout << "seed: " << seed << "\n";
    std::cout << "nfes: " << nfes << "\n";
    std::cout << "runtime (s): " << runtime << "\n";
    std::cout << "speed (evals/s): " << (nfes / runtime) << "\n";
    // Round fitness of best result to 6 places
    std::cout << "E: " << std::fixed <<
        std::setprecision(6) << calculateFitness(best.coords) << "\n";
    // Print coordinates of best solution
    fixZeroes(best.coords);
    std::cout << "solution: "; printCoords(best);
}