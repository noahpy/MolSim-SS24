
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

void force_stroemer_verlet_rv(const Simulation& sim)
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

void force_stroemer_verlet_V2(const Simulation& sim)
{
    // map for indices of the particle container
    std::unordered_map<const Particle*, size_t> particleIndexMap;
    const auto& particles = sim.container.getContainer();
    for (size_t i = 0; i < particles.size(); ++i) {
        particleIndexMap[&particles[i]] = i;
    }

    // Force vector for storing computed F_i
    std::vector<std::array<double, 3>> f_i(sim.container.getContainer().size(), { 0.0, 0.0, 0.0 });

    // Force Calculation with unique pairs by adding F_ij and F_ji = - F_ij to the right forces
    for (auto it = sim.container.beginPairs(); it != sim.container.endPairs(); ++it) {
        auto pair = *it;
        Particle& p1 = pair.first;
        Particle& p2 = pair.second;

        size_t index1 = particleIndexMap[&p1];
        size_t index2 = particleIndexMap[&p2];

        double m_mul = p1.getM() * p2.getM();
        double dist = std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3);
        double coeff = m_mul / dist;
        std::array<double, 3> f_ij = coeff * (p2.getX() - p1.getX());

        f_i[index1] = f_i[index1] + f_ij;
        f_i[index2] = f_i[index2] - f_ij;
    }

    // Storing computed Forces to particles
    int k = 0;
    for (auto& p : sim.container) {
        p.setOldF(p.getF());
        p.setF(f_i[k]);
        ++k;
    }
}
