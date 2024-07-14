
#include "MembraneSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"

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
    const BoundaryConfig& boundaryConfig,
    double gravity_constant,
    double T_init,
    double T_target,
    double delta_T,
    std::vector<std::unique_ptr<Molecule>> molecules,
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
          false,
          n_thermostat,
          doProfile)
    , molecules(std::move(molecules))
{
    if (read_file) {
        this->reader->readFile(*this);
        cellGrid.addParticlesFromContainer(container);
    }

    size_t molCount = 1;
    for (auto& molecule : molecules) {
        molecule->generateMolecule(container, molCount++);
    }
}

void MembraneSimulation::runSim()
{
    // Simply run parent simulation
    MixedLJSimulation::runSim();
}
