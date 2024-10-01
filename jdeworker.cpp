#include "jdeworker.h"
#include "jde.h"

#include <sstream>
#include <iomanip>

JDEWorker::JDEWorker(QObject *parent) : QObject{parent}, runInstance(true) {}

void JDEWorker::setArguments(unsigned int N, unsigned int seed, unsigned int nfesLmt, unsigned int runtimeLmt,
                             unsigned int Np, double target, bool optimizeZeroes)
{
    this->N = N;
    this->seed = seed;
    this->nfesLmt = nfesLmt;
    this->runtimeLmt = runtimeLmt;
    this->Np = Np;
    this->target = target;
    this->optimizeZeroes = optimizeZeroes;
}

void JDEWorker::startAlgorithm(X& best, std::vector<X>& population, unsigned int& nfes, std::mt19937& generator)
{
    std::vector<double> V(3 * N, 0.0), U(3 * N, 0.0);

    auto start = std::chrono::steady_clock::now();

    double currentFitness = 0.0;
    double bestFitness = 0.0;
    while (runInstance && (bestFitness > target + epsilon)) {
        for (int i = 0; i < population.size(); i++) {
            double oldF = population[i].F;
            double oldCr = population[i].Cr;

            mutate(population, V, i, Np, generator);
            crossover(population, V, U, i, Np, generator);
            repair(U);
            currentFitness = select(population, U, i, oldF, oldCr, optimizeZeroes);

            currentFitness = round(currentFitness * 1e6) / 1e6;
            //std::cout << currentFitness << "\n";
            // Increment the number of evaluations
            nfes++;

            // Updates best
            if (currentFitness < bestFitness) {
                best = population[i];
                bestFitness = currentFitness;
                // Emit the new energy value to update the UI
                auto end = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                // Time in seconds
                emit fitnessUpdated(bestFitness, (elapsed/1000.0));
            }
        }


        // Nfes limit check
        if (nfes >= nfesLmt) {
            std::cout << "Nfes limit reached.\n";
            break;
        }

        // Runtime limit check, duration in seconds
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        // Runtime limit reached or exceeded
        if (runtimeLmt != -1 && elapsed >= runtimeLmt) {
            //std::cout << "Time limit exceeded.\n";
            break;
        }
    }
    auto end = std::chrono::steady_clock::now();
    // Duration in seconds
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    // If fitness of best X is lower than (target + epsilon), it is a favorable solution
    bool isTargetReached = (bestFitness <= target + epsilon);

    // Compile end results and signal the UI
    if (optimizeZeroes) { fixZeroes(best.coords); }
    std::string coordsStr = coordsToString(best);

    std::stringstream ss;
    // Solution display has a weird glitch, maybe put in right side next to graph
    ss << (isTargetReached ? "TARGET REACHED. \n" : "TARGET NOT REACHED. \n") << "N: " << N << "\nseed: " << seed << "\nnfes: "
       << nfes << "\nruntime (s): " << duration << "s" << "\nspeed (nfes/s): " << (nfes/duration)
       << "\nE: " << std::fixed << std::setprecision(6) << bestFitness;
    std::string algoResults = ss.str();

    emit sendAlgorithmResults(algoResults, duration);

    // Signal so thread can be quit
    emit taskFinished();
}

void JDEWorker::initializeAlgorithm(){
    // Algorithm setup
    std::mt19937 generator(seed);
    std::vector<X> population;
    unsigned int nfes = 0;

    // Initialize a population and insert random doubles in atoms of each X
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

    startAlgorithm(best, population, nfes, generator);
}

void JDEWorker::terminateExecution(){
    runInstance = false;
}
