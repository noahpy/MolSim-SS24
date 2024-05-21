
#include "simulation/planetSim.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include <memory>
#include <spdlog/spdlog.h>

PlanetSimulation::PlanetSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader)
    : Simulation(time, delta_t, end_time, container, strat, std::move(writer), std::move(reader))
{
    this->reader->readFile(*this);
}

void PlanetSimulation::runSim()
{
    while (time < end_time) {
        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        ++iteration;
        if (iteration % 10 == 0) {
            writer->plotParticles(*this);
        }
        spdlog::debug("Iteration: {}", iteration);

        time += delta_t;
    }
}
