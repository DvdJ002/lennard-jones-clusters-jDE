#ifndef X_H
#define X_H

#include <iostream>
#include <vector>

class X {
public: std::vector<double> coords;
    double F, Cr;

    X(unsigned int N) : coords(3 * N), F(0.5), Cr(0.9) {}
};

#endif // X_H
