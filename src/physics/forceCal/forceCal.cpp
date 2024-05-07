
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
