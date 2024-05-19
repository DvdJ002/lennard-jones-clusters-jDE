#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

#include "utils.h"
#include "X.h"
#include "jDE.h"

// Test getRandomDouble to ensure it generates values between 0 and 1
void test_getRandomDouble() {
    std::mt19937 generator(std::random_device{}());
    for (int i = 0; i < 100; ++i) {
        double random = getRandomDouble(generator);
        assert(random >= 0 && random <= 1);
    }
    std::cout << "Test getRandomDouble passed.\n";
}

// Test calculateDistance function with known coordinates
void test_calculateDistance() {
    double distance = calculateDistance(0, 0, 0, 1, 1, 1);
    assert(distance == 3); // Expected distance: sqrt(3)
    std::cout << "Test calculateDistance passed.\n";
}

// Test calculateFitness function with a simple input
void test_calculateFitness() {
    std::vector<double> coords = {0, 0, 0, 1, 1, 1};
    double fitness = calculateFitness(coords, 2);
    assert(fitness == 0); // Expected fitness: 0
    std::cout << "Test calculateFitness passed.\n";
}

// Test initializePopulation to ensure it initializes the population with random values
void test_initializePopulation() {
    std::vector<X> population;
    std::mt19937 generator(std::random_device{}());
    initializePopulation(population, generator, 5, 10);
    assert(population.size() == 10);
    for (const auto& item : population) {
        for (int i = 0; i < item.coords.size(); ++i) {
            assert(item.coords[i] >= XjL && item.coords[i] <= XjU);
        }
    }
    std::cout << "Test initializePopulation passed.\n";
}


int main() {
    test_getRandomDouble();
    test_calculateDistance();
    test_calculateFitness();
    test_initializePopulation();
    return 0;
}
