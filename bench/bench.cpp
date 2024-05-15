
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/generators/CuboidParticleCluster.h"
#include "models/generators/ParticleGenerator.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include <benchmark/benchmark.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

static void BM_LJSimulation(benchmark::State& state)
{

    spdlog::set_level(spdlog::level::off);

    // Initialize reader
    EmptyFileReader fileReader("");
    // Initialize writer
    outputWriter::VTKWriter writer;

    auto readerPointer = std::make_unique<EmptyFileReader>(fileReader);
    auto writerPointer = std::make_unique<outputWriter::VTKWriter>(writer);

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_lennard_jones };

    // Intialize empty particle container
    ParticleContainer particles { {} };

    // Setup simulation
    LennardJonesSimulation sim (0, 0.0002, 5, particles, strat, std::move(writerPointer), std::move(readerPointer), 5, 1 );

    CuboidParticleCluster cluster1 = CuboidParticleCluster(
        std::array<double, 3> { 0, 0, 0 },
        40,
        8,
        1,
        1.1225,
        1,
        std::array<double, 3> { 0, 0, 0 },
        0.1,
        2);
    CuboidParticleCluster cluster2 = CuboidParticleCluster(
        std::array<double, 3> { 15, 15, 0 },
        8,
        8,
        1,
        1.1225,
        1,
        std::array<double, 3> { 0, -10, 0 },
        0.1,
        2);

    ParticleGenerator p = ParticleGenerator(sim.container);
    p.registerCluster(std::make_unique<CuboidParticleCluster>(cluster1));
    p.registerCluster(std::make_unique<CuboidParticleCluster>(cluster2));
    p.generateClusters();


    for (auto _ : state) {
        sim.time = 0;
        sim.runSim();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_LJSimulation);
