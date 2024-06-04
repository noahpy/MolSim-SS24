
#include "LennardJonesDomainSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include <spdlog/spdlog.h>

LennardJonesDomainSimulation::LennardJonesDomainSimulation(
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
    double cutoff,
    BoundaryConditionHandler& bcHandler,
    unsigned frequency,
    unsigned updateFrequency,
    bool read_file)
    : LinkedLennardJonesSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          epsilon,
          sigma,
          domainOrigin,
          domainSize,
          cutoff,
          frequency,
          updateFrequency,
          read_file)
    , bcHandler(bcHandler)
{
}

void LennardJonesDomainSimulation::runSim()
{
    while (time < end_time) {
        bcHandler.preUpdateBoundaryHandling(*this);

        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        bcHandler.postUpdateBoundaryHandling(*this);

        ++iteration;
        if (iteration % frequency == 0) {
            writer->plotParticles(*this);
        }
        if (iteration % updateFrequency == 0) {
            cellGrid.updateCells();
        }
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}
