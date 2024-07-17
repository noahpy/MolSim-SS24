
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
    std::map<unsigned , bool> stationaryParticleTypes,
    const std::map<unsigned, std::pair<double, double>>& LJParams,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    const BoundaryConfig& boundaryConfig,
    std::unique_ptr<Analyzer> analyzer,
    double gravity_constant,
    std::unique_ptr<Thermostat> thermostat,
    std::vector<std::unique_ptr<Molecule>> molecules_arg,
    unsigned frequency,
    unsigned updateFrequency,
    size_t analysisFrequency,
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
          std::move(stationaryParticleTypes),
          LJParams,
          domainOrigin,
          domainSize,
          cutoff,
          boundaryConfig,
          std::move(analyzer),
          gravity_constant,
          std::move(thermostat),
          frequency,
          updateFrequency,
          analysisFrequency,
          false,
          n_thermostat,
          doProfile)
    , molecules(std::move(molecules_arg))
{
    if (read_file) {
        this->reader->readFile(*this);
    }

    size_t molCount = 1;
    for (auto& molecule : molecules) {
        molecule->generateMolecule(container, molCount++);
        unsigned ptype = molecule->getPtype();
        molecule->initLJParams(epsilons[{ptype, ptype}], sigmas[{ptype, ptype}]);
    }

    cellGrid.addParticlesFromContainer(container);
}

void MembraneSimulation::runSim()
{
    // Simply run parent simulation
    MixedLJSimulation::runSim();
}
