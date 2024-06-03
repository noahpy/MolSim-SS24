
#include "linkedLennardJonesSim.h"
#include "io/fileWriter/FileWriter.h"
#include "io/fileReader/FileReader.h"
#include "physics/strategy.h"
#include <spdlog/spdlog.h>

LinkedLennardJonesSimulation::LinkedLennardJonesSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    double epsilon,
    double sigma,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff)
    : LennardJonesSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          epsilon,
          sigma)
    , cellGrid(domainOrigin, domainSize, cutoff)
{
}

void LinkedLennardJonesSimulation::runSim()
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