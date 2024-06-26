
#include "physics/forceCal/forceCal.h"
#include "models/linked_cell/CellGrid.h"
#include "simulation/baseSimulation.h"
#include "utils/ArrayUtils.h"

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

void lj_calc(
    Particle& p1,
    Particle& p2,
    double alpha,
    double beta,
    double gamma,
    std::array<double, 3> delta)
{
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
        lj_calc(
            pair.first, pair.second, alpha, beta, gamma, pair.first.getX() - pair.second.getX());
    }
}

void force_lennard_jones_lc(const Simulation& sim)
{
    const LinkedLennardJonesSimulation& len_sim =
        static_cast<const LinkedLennardJonesSimulation&>(sim);

    // these values are constant for the simulation and are precomputed
    double alpha = len_sim.getAlpha();
    double beta = len_sim.getBeta();
    double gamma = len_sim.getGamma();

    const CellGrid& cellGrid = len_sim.getGrid();

    // for all cells in the grid
    for (size_t x = 1; x < cellGrid.cells.size() - 1; ++x) {
        for (size_t y = 1; y < cellGrid.cells[0].size() - 1; ++y) {
            // This bool controls the 2D case, where we do need to calc the forces
            // This will be true iff the simulation is 2D
            // Then the condition of the loop will be true and the loop will be executed
            // We then set it to false to not run it further. -> Remember to set z=0
            bool doLoopFor2D = cellGrid.cells[0][0].size() == 1;

            for (size_t z = 1; z < cellGrid.cells[0][0].size() - 1 || doLoopFor2D; ++z) {
                if (doLoopFor2D) {
                    doLoopFor2D = false; // only do it once
                    z = 0;
                }

                // calculate the LJ forces in the cell
                for (auto it = cellGrid.cells.at(x).at(y).at(z)->beginPairs();
                     it != cellGrid.cells.at(x).at(y).at(z)->endPairs();
                     ++it) {
                    auto pair = *it;
                    std::array<double, 3> delta = pair.first.getX() - pair.second.getX();
                    // check if the distance is less than the cutoff
                    if (ArrayUtils::L2Norm(delta) <= len_sim.getGrid().cutoffRadius)
                        lj_calc(pair.first, pair.second, alpha, beta, gamma, delta);
                }
                // calculate LJ forces with the neighbours
                std::list<CellIndex> neighbors = cellGrid.getNeighbourCells({ x, y, z });
                for (auto i : neighbors) {
                    // for all particles in the cell
                    for (auto p1 : cellGrid.cells.at(x).at(y).at(z)->getParticles()) {
                        // go over all particles in the neighbour
                        for (auto p2 : cellGrid.cells[i[0]][i[1]][i[2]]->getParticles()) {
                            // and check if the distance is less than the cutoff
                            std::array<double, 3> delta = p1.get().getX() - p2.get().getX();
                            if (ArrayUtils::L2Norm(delta) <= len_sim.getGrid().cutoffRadius)
                                // then calculate the force
                                lj_calc(p1, p2, alpha, beta, gamma, delta);
                        }
                    }
                }
            }
        }
    }
}
