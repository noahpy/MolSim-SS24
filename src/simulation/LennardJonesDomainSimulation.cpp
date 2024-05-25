
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
    CellGrid& domain,
    BoundaryConditionHandler& bcHandler)
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
    , domain(domain)
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
        if (iteration % 10 == 0) {
            writer->plotParticles(*this);
        }
        spdlog::debug("Iteration {} finished.", iteration);

        time += delta_t;
    }
}
