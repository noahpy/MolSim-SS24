
#include "physics/forceCal/forceCal.h"
#include "utils/ArrayUtils.h"
#include <unordered_map>
#include <vector>

void force_stroemer_verlet(const Simulation& sim)
{
    for (auto& p1 : sim.container) {
        std::array<double, 3> f_i = { 0.0, 0.0, 0.0 };
        for (auto& p2 : sim.container) {
            double m_mul = p1.getM() * p2.getM();
            double dist = std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3);
            // check for distance = 0
            if (dist == 0)
                continue;
            double coeff = m_mul / dist;
            // f_i = f_i + f_ij
            f_i = f_i + coeff * (p2.getX() - p1.getX());
        }
        p1.setOldF(p1.getF());
        p1.setF(f_i);
    }
}

void force_stroemer_verlet_V2(const Simulation& sim)
{
    std::array<double, 3> zeros { 0, 0, 0 };
    for (auto& p : sim.container) {
        p.setOldF(p.getF());
        p.setF(zeros);
    }

    for (auto it = sim.container.beginPairs(); it != sim.container.endPairs(); ++it) {
        auto pair = *it;
        Particle& p1 = pair.first;
        Particle& p2 = pair.second;

        double m_mul = p1.getM() * p2.getM();
        double dist = std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3);
        double coeff = m_mul / dist;

        std::array<double, 3> distance = coeff * (p2.getX() - p1.getX());
        p1.setF(p1.getF() + distance);
        p2.setF(p2.getF() - distance);
    }
}

void force_lennard_jones(const Simulation& sim)
{
    std::array<double, 3> zeros { 0, 0, 0 };
    for (auto& p : sim.container) {
        p.setOldF(p.getF());
        p.setF(zeros);
    }

    for (auto it = sim.container.beginPairs(); it != sim.container.endPairs(); ++it) {
        auto pair = *it;
        Particle& p1 = pair.first;
        Particle& p2 = pair.second;

        // This is constant => maybe we should move this into a class to set it up in the constructor
        double epsilon = 1.0; // set in simulation i.e. access here by sim.epsilon
        double sigma = 1.0; // set in simulation i.e. access here by sim.sigma
        double alpha = -24 * epsilon;
        double beta = std::pow(sigma, 6);
        double gamma = -2 * std::pow(beta, 2);

        std::array<double, 3> delta = p1.getX()-p2.getX();
        double dotDelta = ArrayUtils::DotProduct(p1.getX());
        double dotDelta3 = std::pow(dotDelta, 3);
        double dotDelta6 = std::pow(dotDelta3, 2);

        std::array<double, 3> force = (alpha/dotDelta) * (beta / dotDelta3 + gamma / dotDelta6) * delta;
        p1.setF(p1.getF() + force);
        p2.setF(p2.getF() - force);
    }
}
