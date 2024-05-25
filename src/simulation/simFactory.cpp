
#include "simFactory.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "simulation/lennardJonesSim.h"
#include "simulation/planetSim.h"
#include <spdlog/spdlog.h>

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
    std::unique_ptr<FileReader> readPointer)
{
    switch (sim_type) {
    case 0:
        spdlog::info("Initializing Planet Simulation...");
        return std::make_unique<PlanetSimulation>(
            start_time,
            delta_t,
            end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer));
    case 1:
        spdlog::info("Initializing LJ Simulation...");
        return std::make_unique<LennardJonesSimulation>(
            start_time,
            delta_t,
            end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            epsilon,
            sigma);
    default:
        spdlog::error("Unknown simulation type: {}", sim_type);
        exit(EXIT_FAILURE);
    }
}
