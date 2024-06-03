
#include "simFactory.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "simulation/linkedLennardJonesSim.h"
#include "simulation/planetSim.h"
#include <spdlog/spdlog.h>

std::unique_ptr<Simulation> simFactory(
    Params& params,
    ParticleContainer& particles,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writePointer,
    std::unique_ptr<FileReader> readPointer)
{
    switch (params.simulation_type) {
    case 0:
        spdlog::info("Initializing Planet Simulation...");
        return std::make_unique<PlanetSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer));
    case 1:
        spdlog::info("Initializing LJ Simulation...");
        return std::make_unique<LennardJonesSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            params.epsilon,
            params.sigma);
    case 2:
        return std::make_unique<LinkedLennardJonesSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            params.epsilon,
            params.sigma,
            params.domain_origin,
            params.domain_size,
            params.cutoff
        );
    default:
        spdlog::error("Unknown simulation type: {}", params.simulation_type);
        exit(EXIT_FAILURE);
    }
}
