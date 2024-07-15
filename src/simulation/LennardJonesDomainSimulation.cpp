
#include "LennardJonesDomainSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/strategy.h"
#include <spdlog/spdlog.h>
#include "analytics/Analyzer.h"

#include <utility>

LennardJonesDomainSimulation::LennardJonesDomainSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    std::map<unsigned , bool> stationaryParticleTypes,
    double epsilon,
    double sigma,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    const BoundaryConfig& boundaryConfig,
    std::unique_ptr<Analyzer> analyzer,
    unsigned frequency,
    unsigned updateFrequency,
    size_t analysisFrequency,
    bool read_file)
    : LinkedLennardJonesSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          std::move(stationaryParticleTypes),
          epsilon,
          sigma,
          domainOrigin,
          domainSize,
          cutoff,
          frequency,
          updateFrequency,
          false)
    , bcHandler(boundaryConfig, cellGrid)
    , repulsiveDistance(std::pow(2, 1 / 6) * sigma)
    , analysisFrequency(analysisFrequency)
    , analyzer(std::move(analyzer))
{
    if (read_file) {
        this->reader->readFile(*this);
        cellGrid.addParticlesFromContainer(container);
    }
}

void LennardJonesDomainSimulation::runSim()
{
    if (bcHandler.dimensionality != cellGrid.gridDimensionality) {
        spdlog::error(
            "Dimensionality mismatch between boundary conditions and cell grid.",
            "Boundary conditions: {}, Cell grid: {}",
            bcHandler.dimensionality,
            cellGrid.gridDimensionality);
        exit(EXIT_FAILURE);
    }

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
        if (iteration % analysisFrequency == 0) {
            analyzer->analyze(*this);
        }
        progressLogger.logProgress(iteration);
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}

double LennardJonesDomainSimulation::getRepulsiveDistance(int type) const {
    spdlog::trace("Got repulsive distance from Domain sim");
    return repulsiveDistance;
};
