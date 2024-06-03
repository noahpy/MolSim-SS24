
#include "physics/forceCal/forceCal.h"
#include "simulation/baseSimulation.h"
#include "models/linked_cell/CellGrid.h"
#include "utils/ArrayUtils.h"

class LinkedLennardJonesSimulation;

void force_gravity(const Simulation& sim)
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

void force_gravity_V2(const Simulation& sim)
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

void lj_calc(Particle& p1, Particle& p2, double alpha, double beta, double gamma)
{
    std::array<double, 3> delta = p1.getX() - p2.getX();
    double dotDelta = ArrayUtils::DotProduct(delta);
    double dotDelta3 = std::pow(dotDelta, 3);
    double dotDelta6 = std::pow(dotDelta3, 2);

    // The formula has been rearranged to avoid unnecessary calculations; please see the report
    // for more details
    std::array<double, 3> force =
        (alpha / dotDelta) * (beta / dotDelta3 + gamma / dotDelta6) * delta;
    p1.setF(p1.getF() + force);
    p2.setF(p2.getF() - force);
}

void force_lennard_jones(const Simulation& sim)
{
    std::array<double, 3> zeros { 0, 0, 0 };
    for (auto& p : sim.container) {
        p.setOldF(p.getF());
        p.setF(zeros);
    }

    const LennardJonesSimulation& len_sim = static_cast<const LennardJonesSimulation&>(sim);

    // these values are constant for the simulation and are precomputed
    double alpha = len_sim.getAlpha();
    double beta = len_sim.getBeta();
    double gamma = len_sim.getGamma();

    for (auto it = sim.container.beginPairs(); it != sim.container.endPairs(); ++it) {
        auto pair = *it;
        lj_calc(pair.first, pair.second, alpha, beta, gamma);
    }
}

void force_lennard_jones_lc(const Simulation& sim)
{
    const LinkedLennardJonesSimulation& len_sim = static_cast<const LinkedLennardJonesSimulation&>(sim);

    // these values are constant for the simulation and are precomputed
    double alpha = len_sim.getAlpha();
    double beta = len_sim.getBeta();
    double gamma = len_sim.getGamma();

    const CellGrid& cellGrid = len_sim.getGrid();

    for (size_t i = 0; i < cellGrid.cells.size(); ++i) {
        for (size_t y = 0; y < cellGrid.cells[0].size(); ++y) {
            for (size_t z = 0; z < cellGrid.cells[0][0].size(); ++z) {
                Cell& main = *cellGrid.cells[i][y][z];
                for (auto it = main.beginPairs(); it != main.endPairs(); ++it) {
                    auto pair = *it;
                    lj_calc(pair.first, pair.second, alpha, beta, gamma);
                }
                std::list<CellIndex> neighbors = cellGrid.getNeighbourCells({i,y,z});
                for (auto index: neighbors) {
                    Cell& neighbour = *cellGrid.cells[index[0]][index[1]][index[2]];
                    for (auto p1 : main.getParticles()) {
                        for (auto p2 : neighbour) {
                            lj_calc(p1, p2, alpha, beta, gamma);
                        }
                    }
                }
            }
        }
    }
}