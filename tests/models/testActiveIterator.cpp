
#include "analytics/Analyzer.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/thermostat/ThermostatFactory.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/baseSimulation.h"
#include "utils/Params.h"
#include <cmath>
#include <gtest/gtest.h>
#include <omp.h>
#include <strings.h>

class ActiveIteratorTest : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    double PRESICION = 10e-5;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    std::vector<Particle> particleVec = {};
    ParticleContainer particles { particleVec };

    std::array<double, 3> domainOrigin = { 0, 0, 0 };
    std::array<double, 3> domainSize = { 100, 1000, 100 };
    const double cutoff = 2.5;
    MixedLJSimulation sim;

    ActiveIteratorTest()
        : start_time(0)
        , end_time(1)
        , delta_t(1)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , sim(start_time,
              delta_t,
              end_time,
              particles,
              strat,
              std::move(writer),
              std::move(fileReader),
              {},
              { { 0, { 1, 1 } } },
              domainOrigin,
              domainSize,
              cutoff,
              BoundaryConfig(
                  BoundaryType::OUTFLOW,
                  BoundaryType::OUTFLOW,
                  BoundaryType::OUTFLOW,
                  BoundaryType::OUTFLOW,
                  BoundaryType::OUTFLOW,
                  BoundaryType::OUTFLOW),
              std::make_unique<Analyzer>(Analyzer({ 0, 0, 0 }, "")),
              0,
              thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
              0,
              0,
              0)
    {
    }
};

TEST_F(ActiveIteratorTest, testActiveIterator)
{
    auto grid_size = sim.getGrid().getGridDimensions();
    for (size_t i = 0; i < 2000; ++i) {
        auto p = Particle(
            { static_cast<double>(rand() % grid_size.at(0)),
              static_cast<double>(rand() % grid_size.at(1)),
              static_cast<double>(rand() % grid_size.at(2)) },
            { 0, 0, 0 },
            1,
            3);
        sim.container.addParticle(p);
    }

    spdlog::info("Removing...");
    std::list<size_t> removed;
    for (size_t t = 0; t < 500; ++t) {
        size_t index = rand() % sim.container.particles.size();
        bool wasRemoved = !sim.container.particles.at(index).getActivity();
        sim.container.removeParticle(sim.container.particles.at(index));
        if (!wasRemoved) {
            removed.push_back(index);
        }

        size_t count = 0;
        std::vector<size_t> detected;

#pragma omp parallel for
        for (auto& p : sim.container) {
#pragma omp critical
            {
                ++count;
            }
        }

        ASSERT_EQ(count, sim.container.activeParticleCount)
            << "t = " << t << " count = " << count << "\nat index = " << index
            << "\nwasRemoved = " << wasRemoved << "\n"
            << "\ntotal removed: " << removed.size() << "\n";
    }
}
