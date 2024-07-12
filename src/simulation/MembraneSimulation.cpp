
#include "MembraneSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"

#include <physics/forceCal/forceCal.h>

MembraneSimulation::MembraneSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    const std::map<unsigned, std::pair<double, double>>& LJParams,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    std::array<double, 3> membraneOrigin,
    int numParticlesWidth,
    int numParticlesHeight,
    double k,
    double r_0,
    double spacing,
    const BoundaryConfig& boundaryConfig,
    double gravity_constant,
    double T_init,
    double T_target,
    double delta_T,
    unsigned frequency,
    unsigned updateFrequency,
    bool read_file,
    unsigned n_thermostat,
    bool doProfile)
    : MixedLJSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          LJParams,
          domainOrigin,
          domainSize,
          cutoff,
          boundaryConfig,
          gravity_constant,
          T_init,
          T_target,
          delta_T,
          frequency,
          updateFrequency,
          read_file,
          n_thermostat,
          doProfile),
      membraneOrigin(membraneOrigin),
      numParticlesWidth(numParticlesWidth),
      numParticlesHeight(numParticlesHeight),
      k(k),
      r_0(r_0),
      spacing(spacing),
      membrane(membraneOrigin, numParticlesWidth, numParticlesHeight)
{
    membrane.initMembrane(container, spacing);
}

void MembraneSimulation::runSim()
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
        if (n_thermostat && iteration % n_thermostat == 0) {
            thermostat.updateT(this->container);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}
