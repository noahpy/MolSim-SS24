
#include "simFactory.h"
#include "analytics/Analyzer.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "simulation/MembraneSimulation.h"
#include "simulation/MixedLJSimulation.h"
#include "simulation/planetSim.h"

#include <spdlog/spdlog.h>

std::unique_ptr<Simulation> simFactory(
    Params& params,
    ParticleContainer& particles,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writePointer,
    std::unique_ptr<FileReader> readPointer,
    std::unique_ptr<Thermostat> thermostat)
{
    bool is2DTmp;
    switch (params.simulation_type) {
    case SimulationType::PLANET:
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
            std::move(params.immobileParticleTypes),
            params.plot_frequency);

    case SimulationType::LJ:
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
            std::move(params.immobileParticleTypes),
            params.epsilon,
            params.sigma,
            params.plot_frequency);
    case SimulationType::LINKED_LJ:
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
            std::move(params.immobileParticleTypes),
            params.epsilon,
            params.sigma,
            params.domain_origin,
            params.domain_size,
            params.cutoff,
            params.plot_frequency,
            params.update_frequency);
    case SimulationType::DOMAIN_LJ:
        spdlog::info("Initializing Linked and domain bound LJ Simulation with:");
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
        is2DTmp = params.boundaryConfig.boundaryMap.size() == 4;
        spdlog::info(
            "left boundary: {}, right boundary: {}, top boundary: {}, bottom boundary: {}, front "
            "boundary: {}, back boundary: {}",
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::LEFT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::RIGHT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::TOP)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BOTTOM)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::FRONT)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BACK)));

        return std::make_unique<LennardJonesDomainSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            std::move(params.immobileParticleTypes),
            params.epsilon,
            params.sigma,
            params.domain_origin,
            params.domain_size,
            params.cutoff,
            params.boundaryConfig,
            std::make_unique<Analyzer>(params.bins, params.outName),
            params.plot_frequency,
            params.update_frequency,
            params.analysisInterval,
            true);
    case SimulationType::MIXED_LJ:
        spdlog::info("Initializing Mixed LJ + Gravity Simulation with:");
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
        is2DTmp = params.boundaryConfig.boundaryMap.size() == 4;
        spdlog::info(
            "left boundary: {}, right boundary: {}, top boundary: {}, bottom boundary: {}, front "
            "boundary: {}, back boundary: {}",
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::LEFT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::RIGHT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::TOP)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BOTTOM)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::FRONT)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BACK)));

        return std::make_unique<MixedLJSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            std::move(params.immobileParticleTypes),
            params.typesMap,
            params.domain_origin,
            params.domain_size,
            params.cutoff,
            params.boundaryConfig,
            std::make_unique<Analyzer>(params.bins, params.outName),
            params.gravity,
            std::move(thermostat),
            params.plot_frequency,
            params.update_frequency,
            params.analysisInterval,
            true,
            params.thermo_freq,
            params.doPerformanceMeasurements);
    case SimulationType::MEMBRANE_LJ:
        spdlog::info("Initializing Membrane Simulation with:");
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
        is2DTmp = params.boundaryConfig.boundaryMap.size() == 4;
        spdlog::info(
            "left boundary: {}, right boundary: {}, top boundary: {}, bottom boundary: {}, front "
            "boundary: {}, back boundary: {}",
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::LEFT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::RIGHT)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::TOP)),
            getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BOTTOM)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::FRONT)),
            is2DTmp ? "None (2D)"
                    : getBoundaryString(params.boundaryConfig.boundaryMap.at(Position::BACK)));

        return std::make_unique<MembraneSimulation>(
            params.start_time,
            params.delta_t,
            params.end_time,
            particles,
            strat,
            std::move(writePointer),
            std::move(readPointer),
            std::move(params.immobileParticleTypes),
            params.typesMap,
            params.domain_origin,
            params.domain_size,
            params.cutoff,
            params.boundaryConfig,
            std::make_unique<Analyzer> (params.bins, params.outName),
            params.gravity,
            std::move(thermostat),
            std::move(params.molecules),
            params.plot_frequency,
            params.update_frequency,
            params.analysisInterval,
            true,
            params.thermo_freq,
            params.doPerformanceMeasurements);
    }

    spdlog::error("Invalid simulation type.");
    exit(EXIT_FAILURE);
}
