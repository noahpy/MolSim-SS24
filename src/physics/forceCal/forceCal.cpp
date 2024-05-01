
#include "physics/forceCal/forceCal.h"
#include "utils/ArrayUtils.h"
#include <vector>
#include <iostream>

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
    int k = 0;
    int j = 1;
    std::vector<std::array<double, 3>> f_i;
    for (auto& p : sim.container) {
        f_i.push_back(std::array<double,3> {0, 0, 0});
    }
    std::array<double, 3> f_ij = {0, 0, 0};
    for (auto it = sim.container.beginPairs(); it != sim.container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pair = *it;
		double m_mul = pair.first.getM() * pair.second.getM();
        double dist = std::pow(ArrayUtils::L2Norm(pair.first.getX() - pair.second.getX()), 3);
        double coeff = m_mul / dist;
        f_ij = coeff * (pair.second.getX() - pair.first.getX());
		std::cout << "f_ij = " << f_ij << "\n";
        f_i[k+j] = f_i[k+j] - f_ij;
		std::cout << "f[" << k+j << "] = " << f_i[k+j] << "\n";
        f_i[k] = f_i[k] + f_ij;
		std::cout << "f[" << k << "] = " << f_i[k] << "\n";
        if (it == sim.container.endPairs()) {
            sim.container.getContainer()[k+1].setOldF(sim.container.getContainer()[k+1].getF());
            sim.container.getContainer()[k+1].setF(f_i[k+1]);
        }
        if (pair.second == it.getLast()){
            sim.container.getContainer()[k].setOldF(sim.container.getContainer()[k].getF());
            sim.container.getContainer()[k].setF(f_i[k]);
            ++k;
            j = 1;
        } else {
            ++j;
        }
    }
}