#ifndef VAJA_2_JDE_H
#define VAJA_2_JDE_H
#include <vector>
#include <random>
#include "utils.h"

void mutate(std::vector<X>& population, std::vector<double>& V, int i,
    unsigned int Np, std::mt19937& generator)
{
    if (getRandomDouble(generator) < 0.1) {
        population[i].F = 0.1 + getRandomDouble(generator) * 0.9;
    }

    std::vector<int> indices = generateDistinctIndices(Np, i);
    int r1 = indices[0]; int r2 = indices[1]; int r3 = indices[2];

    for (int j = 0; j < population[i].coords.size(); ++j) {
        V[j] = population[r1].coords[j] +
            (population[i].F * (population[r2].coords[j] - population[r3].coords[j]));
    }
}

void crossover(std::vector<X>& population, std::vector<double>& V, std::vector<double>& U,
    int i, unsigned int Np, std::mt19937& generator)
{
    if (getRandomDouble(generator) < 0.1) {
        population[i].Cr = getRandomDouble(generator);
    }

    int jRand = (int)(getRandomDouble(generator) * (double)(population[i].coords.size()));

    for (int j = 0; j < population[i].coords.size(); ++j) {
        if (getRandomDouble(generator) < population[i].Cr || jRand == j) {
            U[j] = V[j];
        }
        else {
            U[j] = population[i].coords[j];
        }
    }
}

void repair(std::vector<double>& U) {
    for (int j = 0; j < U.size(); j++) {
        if (U[j] < XjL) {
            U[j] = XjL;
        }
        else if (U[j] > XjU) {
            U[j] = XjU;
        }
    }
}

double select(std::vector<X>& population, std::vector<double>& U, int i, double oldF, double oldCr) {
    double fitnessU = calculateFitness(U);
    double fitnessX = calculateFitness(population[i].coords);

    if (fitnessU < fitnessX) {
        population[i].coords = U;
        return fitnessU;
    }
    population[i].F = oldF; population[i].Cr = oldCr;
    return fitnessX;
}

auto startAlgorithm(
    X& best, std::vector<X>& population, unsigned int N,
    unsigned int Np, unsigned int& nfes, unsigned int nfesLimit, double runtimeLimit,
    std::mt19937& generator, double target)
{
    std::vector<double> V(3 * N, 0.0), U(3 * N, 0.0);

    auto start = std::chrono::steady_clock::now();

    double currentFitness = 0.0;
    double bestFitness = 0.0;
    while (bestFitness > target + epsilon) {
        for (int i = 0; i < population.size(); i++) {
            double oldF = population[i].F;
            double oldCr = population[i].Cr;

            mutate(population, V, i, Np, generator);
            crossover(population, V, U, i, Np, generator);
            repair(U);
            currentFitness = select(population, U, i, oldF, oldCr);

            // Round the fitness value to 6 decimal places
            currentFitness = round(currentFitness * 1e6) / 1e6;
            //std::cout << currentFitness << "\n";
            // Increment the number of evaluations
            nfes++;

            // Updates best
            if (currentFitness < bestFitness) {
                best = population[i];
                bestFitness = currentFitness;
                std::cout << bestFitness << "\n";
            }
        }


        // Nfes limit check
        if (nfes >= nfesLimit) {
            std::cout << "Nfes limit reached.\n";
            break;
        }

        // Runtime limit check, duration in seconds
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        // Runtime limit reached or exceeded
        if (runtimeLimit != -1 && elapsed >= runtimeLimit) {
            std::cout << "Time limit exceeded.\n";
            break;
        }
    }
    auto end = std::chrono::steady_clock::now();
    // Duration in seconds
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    // If fitness of best X is lower than (target + epsilon), it is a favorable solution
    if (bestFitness <= target + epsilon) {
        std::cout << "Target reached.\n";
    }
    else {
        std::cout << "Target not reached.\n";
    }

    return duration;
}

#endif //VAJA_2_JDE_H