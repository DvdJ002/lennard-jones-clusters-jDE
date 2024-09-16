#ifndef JDE_H
#define JDE_H

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

double select(std::vector<X>& population, std::vector<double>& U, int i, double oldF, double oldCr, bool fixZeroes) {
    double fitnessU = calculateFitness(U, fixZeroes);
    double fitnessX = calculateFitness(population[i].coords, fixZeroes);

    if (fitnessU < fitnessX) {
        population[i].coords = U;
        return fitnessU;
    }
    population[i].F = oldF; population[i].Cr = oldCr;
    return fitnessX;
}

#endif // JDE_H
