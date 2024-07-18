
#pragma once
#include "simulation/baseSimulation.h"

/**
 * @brief Calculate the forces between particles using the Stroemer-Verlet algorithm
 * @param sim The simulation object to calculate the forces for
 * @return void
 */
void force_gravity(const Simulation& sim);

/**
 * @brief Calculate the forces between particles using the Stroemer-Verlet algorithm, while
 * exploiting F_12 = -F_21 and iterating over distinct pairs
 * @param sim The simulation object to calculate the forces for
 * @return void
 */
void force_gravity_V2(const Simulation& sim);

/**
 * @brief Calculate the forces between particles using the Lennard-Jones potential
 * @param sim The Lennard Jones simulation object to calculate the forces for
 * @return void
 */
void force_lennard_jones(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential
 * @param sim The Lennard Jones simulation object to calculate the forces for
 * @return void
 */
void force_lennard_jones_lc(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential and the mixing rules to allow multiple particle types & regard gravity
 * @param sim The simulation to calculate the forces for
 */
void force_mixed_LJ_gravity_lc(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential, the mixing rules to allow multiple particle types & regard gravity and the harmonic
 * potential between particles of a molecules
 * @param sim The simulation to calculate the forces for
 */
void force_membrane(const Simulation& sim);

/**
 * @brief Calculate the forces between 2 particles in a molecule using the harmonic potential
 * @param p1 The first particle
 * @param p2 The second particle
 * @param k The spring constant
 * @param r_0 The equilibrium distance
 */
void harmonic_calc(Particle& p1, Particle& p2, double k, double r_0);

/**
 * @brief Calculate the forces between 2 particles using the Lennard-Jones potential
 * @param p1 The first particle
 * @param p2 The second particle
 * @param alpha The alpha value for the LJ potential
 * @param beta The beta value for the LJ potential
 * @param gamma The gamma value for the LJ potential
 * @param delta The difference between the two particle positions
 */
void lj_calc(
    Particle& p1,
    Particle& p2,
    double alpha,
    double beta,
    double gamma,
    std::array<double, 3> delta);

void force_mixed_LJ_gravity_lc_task(const Simulation& sim);
