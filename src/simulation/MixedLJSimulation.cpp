
#include "MixedLJSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"

MixedLJSimulation::MixedLJSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    const std::vector<std::pair<int, double>>& epsilons,
    const std::vector<std::pair<int, double>>& sigmas,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    const BoundaryConfig& boundaryConfig,
    double gravity_constant,
    double T_init,
    double T_target,
    double delta_T,
    unsigned int frequency,
    unsigned int updateFrequency,
    bool read_file,
    unsigned int n_thermostat)
    : LennardJonesDomainSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          0,
          0,
          domainOrigin,
          domainSize,
          cutoff,
          boundaryConfig,
          frequency,
          updateFrequency,
          false)
    , gravity_constant(gravity_constant)
    , T_init(T_init)
    , T_target(T_target)
    , delta_T(delta_T)
    , n_thermostat(n_thermostat)
{
    if (read_file) {
        this->reader->readFile(*this);
        cellGrid.addParticlesFromContainer(container);
    }

    // Insert all combinations of epsilons into the map
    for (auto epsilonOuter : epsilons) {
        int type1 = epsilonOuter.first;
        double epsilon1 = epsilonOuter.second;
        for (auto epsilonInner : epsilons) {
            int type2 = epsilonInner.first;
            double epsilon2 = epsilonInner.second;

            if (this->epsilons.find(getMixKey(type1, type2)) == this->epsilons.end())
                this->epsilons.insert({ getMixKey(type1, type2), std::sqrt(epsilon1 * epsilon2) });
        }
    }
    // Insert all combinations of sigmas into the map
    for (auto sigmaOuter : sigmas) {
        int type1 = sigmaOuter.first;
        double sigma1 = sigmaOuter.second;
        for (auto sigmaInner : sigmas) {
            int type2 = sigmaInner.first;
            double sigma2 = sigmaInner.second;

            if (this->sigmas.find(getMixKey(type1, type2)) == this->sigmas.end())
                this->sigmas.insert({ getMixKey(type1, type2), (sigma1 + sigma2) / 2 });
        }
    }
    // Insert all combinations of alphas into the map -> to use in LJ force
    for (auto epsilonPair : this->epsilons) {
        std::pair<int, int> key = epsilonPair.first;
        double eps = epsilonPair.second;
        this->alphas.insert({ key, -24 * eps });
    }
    // Insert all combinations of betas & gammas into the map -> to use in LJ force
    for (auto sigmaPair : this->sigmas) {
        std::pair<int, int> key = sigmaPair.first;
        double sig = sigmaPair.second;
        this->betas.insert({ key, std::pow(sig, 6) });
        this->gammas.insert({ key, -2 * std::pow(sig, 12) });
    }
}

void MixedLJSimulation::runSim()
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
        if (iteration % n_thermostat == 0) {
            // thermostat();
        }
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}

std::pair<int, int> MixedLJSimulation::getMixKey(int type1, int type2)
{
    if (type1 > type2) {
        std::swap(type1, type2);
    }
    // Always return the pair with the smaller type first
    return std::make_pair(type1, type2);
}
