
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
        spdlog::info("Initializing Planet Simulation with:");
        spdlog::info(
            "delta_t: {}, end_time: {}, plot_frequency: {}",
            params.delta_t,
            params.end_time,
            params.plot_frequency);
        return std::make_unique<PlanetSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            params.plot_frequency);

    case 1:
        spdlog::info("Initializing LJ Simulation with:");
        spdlog::info(
            "delta_t: {}, end_time: {}, epsilon: {}, sigma: {}, plot_frequency: {}",
            params.delta_t,
            params.end_time,
            params.epsilon,
            params.sigma,
            params.plot_frequency);
        return std::make_unique<LennardJonesSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            params.epsilon,
            params.sigma,
            params.plot_frequency);
    case 2:
        spdlog::info("Initializing Linked LJ Simulation with:");
        spdlog::info(
            "delta_t: {}, end_time: {}, epsilon: {}, sigma: {}, plot_frequency: {}",
            params.delta_t,
            params.end_time,
            params.epsilon,
            params.sigma,
            params.plot_frequency);
        spdlog::info(
            "domain_origin: ({}, {}, {}), domain_size: ({}, {}, {}), cutoff: {}, update_frequency: "
            "{}",
            params.domain_origin[0],
            params.domain_origin[1],
            params.domain_origin[2],
            params.domain_size[0],
            params.domain_size[1],
            params.domain_size[2],
            params.cutoff,
            params.update_frequency);
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
            params.cutoff,
            params.plot_frequency,
            params.update_frequency);
    default:
        spdlog::error("Unknown simulation type: {}", params.simulation_type);
        exit(EXIT_FAILURE);
    }
}
