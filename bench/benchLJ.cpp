
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/emptyWriter.h"
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
    EmptyFileWriter writer;

    auto readerPointer = std::make_unique<EmptyFileReader>(fileReader);
    auto writerPointer = std::make_unique<EmptyFileWriter>(writer);

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_lennard_jones };

    // Intialize empty particle container
    ParticleContainer particles { {} };

    // Setup simulation
    LennardJonesSimulation sim(
        0,
        0.002,
        1,
        particles,
        strat,
        std::move(writerPointer),
        std::move(readerPointer),
        {},
        5,
        1);

    // get particle number
    auto n = state.range(0);
    unsigned width = 100;
    unsigned height = n / 100;

    CuboidParticleCluster cluster = CuboidParticleCluster(
        std::array<double, 3> { 0, 0, 0 },
        width,
        height,
        1,
        1.1225,
        1,
        std::array<double, 3> { 0, 0, 0 },
        0.1,
        2,
        {});

    ParticleGenerator p = ParticleGenerator(sim.container);
    p.registerCluster(std::make_unique<CuboidParticleCluster>(cluster));
    p.generateClusters();

    for (auto _ : state) {
        sim.time = 0;
        sim.runSim();
    }
    state.SetComplexityN(state.range(0));
}
// Register the function as a benchmark
/* BENCHMARK(BM_LJSimulation)->Arg(1000)->Arg(2000)->Arg(4000)->Arg(8000)->Complexity(); */

BENCHMARK(BM_LJSimulation)->DenseRange(1000, 10000, 1000);
