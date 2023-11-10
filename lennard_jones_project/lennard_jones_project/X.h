#ifndef VAJA_2_X_H
#define VAJA_2_X_H
#include <iostream>
#include <vector>

class X {
public: std::vector<double> coords;
      double F, Cr;

      X(unsigned int N) : coords(3 * N), F(0.5), Cr(0.9) {}
};
#endif //VAJA_2_X_H
