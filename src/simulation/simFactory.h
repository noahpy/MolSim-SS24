
#include "models/ParticleContainer.h"
#include "physics/strategy.h"
#include "simulation/baseSimulation.h"

std::unique_ptr<Simulation> simFactory(
    unsigned sim_type,
    double start_time,
    double delta_t,
    double end_time,
    double epsilon,
    double sigma,
    ParticleContainer& particles,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writePointer,
    std::unique_ptr<FileReader> readPointer);
