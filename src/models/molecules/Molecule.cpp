
#include "models/molecules/Molecule.h"
#include <complex>

void Molecule::initLJParams(double epsilon, double sigma) {
    alpha = -24 * epsilon;
    beta = std::pow(sigma, 6);
    gamma = -2 * std::pow(sigma, 12);

    cutoffRadiusSquared = std::pow(2, 2/6) * sigma;
}
