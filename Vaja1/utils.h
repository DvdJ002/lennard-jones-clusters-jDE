#ifndef VAJA_2_UTILS_H
#define VAJA_2_UTILS_H

#include <iostream>
#include <random>
#include "jDE.h"
#include "X.h"

const int XjL = -6, XjU = 6;
const double epsilon = 1e-6;  // 1 * 10^(-6)

// 0,0,0,0,0,P2iz,0,P3iy,P3iz...
void fixZeroes(std::vector<double>& coords) {
    coords[0] = 0.0; coords[1] = 0.0; coords[2] = 0.0;
    coords[3] = 0.0; coords[4] = 0.0; coords[6] = 0.0;
}

// Print coordinates of a certain X
void printCoords(X x) {
    std::cout << "( ";
    for (int i = 0; i < x.coords.size(); i++) {
        std::cout << x.coords[i] << " ";
    }
    std::cout << ")";
}

// Returns squared distance between 2 atoms
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    double dz = z1 - z2;
    return dx * dx + dy * dy + dz * dz;
}

// Fitness = sum of (distance^-12 - distance^-6) for all pairs of atoms
double calculateFitness(std::vector<double> coords) {
    fixZeroes(coords);
    double fitness = 0;
    for (int i = 0; i < 27; i += 3) {
        for (int j = i + 3; j < 30; j += 3) {
            // Calculate the distance between atoms i and j
            double distance_squared =
                calculateDistance(
                    coords[i], coords[i + 1], coords[i + 2],
                    coords[j], coords[j + 1], coords[j + 2]
                );

            // Manual multiplication, sqrt() and pow() make the runtime slower
            double distance_pow_minus12 =
                1.0 / (distance_squared * distance_squared * distance_squared *
                    distance_squared * distance_squared * distance_squared);
            double distance_pow_minus6 =
                1.0 / (distance_squared * distance_squared * distance_squared);

            fitness += (distance_pow_minus12 - distance_pow_minus6);
        }
    }
    return 4 * fitness;
}

// Returns random double between XjL and xjU
double getRandomDouble(std::mt19937& generator) {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Returns 3 distinct random indices, each different from i
std::vector<int> generateDistinctIndices(int bound, int i) {
    std::vector<int> indices;
    int r1, r2, r3;

    do {
        r1 = std::rand() % bound;
    } while (r1 == i);

    do {
        r2 = std::rand() % bound;
    } while (r2 == r1 || r2 == i);

    do {
        r3 = std::rand() % bound;
    } while (r3 == r1 || r3 == r2 || r3 == i);

    indices.push_back(r1);
    indices.push_back(r2);
    indices.push_back(r3);
    return indices;
}

#endif //VAJA_2_UTILS_H
