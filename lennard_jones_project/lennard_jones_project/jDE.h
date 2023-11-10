#ifndef VAJA_2_JDE_H
#define VAJA_2_JDE_H
#include <vector>
#include <random>
#include "utils.h"

// Chance to adjust the mutation factor F, before mutating the current vector 
void mutate(std::vector<X>& population, std::vector<double>& V, int i,
    unsigned int Np, std::mt19937& generator)
{
    // 10% chance to change the F of our XiG
    if (getRandomDouble(generator) < 0.1) {
        population[i].F = 0.1 + getRandomDouble(generator) * 0.9;
    }

    // Mutate the current vector using 3 random other XiG from the population
    std::vector<int> indices = generateDistinctIndices(Np, i);
    int r1 = indices[0]; int r2 = indices[1]; int r3 = indices[2];

    for (int j = 0; j < population[i].coords.size(); ++j) {
        V[j] = population[r1].coords[j] +
            (population[i].F * (population[r2].coords[j] - population[r3].coords[j]));
    }
}

// Chance to change the crossover rate of our population
// and potentially update the target vector U 
void crossover(std::vector<X>& population, std::vector<double>& V, std::vector<double>& U,
    int i, unsigned int Np, std::mt19937& generator)
{   
    // 10% chance to update the crossover rate of our XiG
    if (getRandomDouble(generator) < 0.1) {
        population[i].Cr = getRandomDouble(generator);
    }

    // Update the vector U based on a probability condition
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

// Repair ensures that the updated target vector Uj,G stays within [XjL, XjU].
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

// The next generation solution(Xi, G + 1) is adjusted based on the fitness comparison results
// Function returns the best fitness between XiG and U
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

// Function responsible for the flow, logic, and solution of the jDE algorithm
// The process returns the current best solution if either:
// 1. runtime limit or nfes limit has been reached
// 2. the current best solution is optimal (with an error margin of 1 * 10^(-6))
auto startAlgorithm(
    X& best, std::vector<X>& population, unsigned int N,
    unsigned int Np, unsigned int& nfes, unsigned int nfesLimit, double runtimeLimit,
    std::mt19937& generator, double target)
{
    // Initialize needed variables and start runtime
    std::vector<double> V(3 * N, 0.0), U(3 * N, 0.0);
    auto start = std::chrono::steady_clock::now();
    double currentFitness = 0.0; double bestFitness = 0.0;

    while (bestFitness > target + epsilon) {
        for (int i = 0; i < population.size(); i++) {
            // Save the old F and Cr factors for potential further use
            double oldF = population[i].F; double oldCr = population[i].Cr;

            // mutate -> crossover -> repair -> select; the cycle of the jDE algorithm
            // increment nfes (the number of evaluations) each time
            mutate(population, V, i, Np, generator);
            crossover(population, V, U, i, Np, generator);
            repair(U);
            currentFitness = round(select(population, U, i, oldF, oldCr) * 1e6) / 1e6;
            nfes++;

            // Update best value
            updateBest(population, currentFitness, bestFitness, best, i);
        }
        // break if any limit is reached
        if (isLimitReached(start, nfes, nfesLimit, runtimeLimit)) { break; }
    }
    // Duration in seconds
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    return duration;
}

#endif //VAJA_2_JDE_H