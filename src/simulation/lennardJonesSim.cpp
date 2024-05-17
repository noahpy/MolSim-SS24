
#include "lennardJonesSim.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include <cmath>
#include <spdlog/spdlog.h>

LennardJonesSimulation::LennardJonesSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    double epsilon,
    double sigma)
    : Simulation(time, delta_t, end_time, container, strat, std::move(writer), std::move(reader))
    , epsilon(epsilon)
    , sigma(sigma)
{
    this->alpha = -24 * epsilon;
    this->beta = std::pow(sigma, 6);
    this->gamma = -2 * std::pow(sigma, 12);
}

void LennardJonesSimulation::runSim()
{
    while (time < end_time) {
        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        ++iteration;
        if (iteration % 10 == 0) {
            writer->plotParticles(*this);
        }
        spdlog::debug("Iteration {} finished.", iteration);

        time += delta_t;
    }
}
