
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
    const std::map<unsigned, std::pair<double, double>>& LJParams,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    const BoundaryConfig& boundaryConfig,
    double gravityConstant,
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
    , gravityConstant(gravityConstant)
    , T_init(T_init)
    , T_target(T_target)
    , delta_T(delta_T)
    , n_thermostat(n_thermostat)
{
    if (read_file) {
        this->reader->readFile(*this);
        cellGrid.addParticlesFromContainer(container);
    }

    // Insert all combinations of epsilons and sigmas into the maps
    for (auto LJParamInner : LJParams) {
        unsigned type1 = LJParamInner.first;
        double epsilon1 = LJParamInner.second.first;
        double sigma1 = LJParamInner.second.second;
        for (auto LJParamOuter : LJParams) {
            unsigned type2 = LJParamOuter.first;
            double epsilon2 = LJParamOuter.second.first;
            double sigma2 = LJParamOuter.second.second;

            if (epsilons.find(getMixKey(type1, type2)) == epsilons.end())
                epsilons.insert({ getMixKey(type1, type2), std::sqrt(epsilon1 * epsilon2) });

            if (sigmas.find(getMixKey(type1, type2)) == sigmas.end())
                sigmas.insert({ getMixKey(type1, type2), (sigma1 + sigma2) / 2 });
        }
    }
    // Insert all combinations of alphas into the map -> to use in LJ force
    for (auto epsilonPair : epsilons) {
        std::pair<int, int> key = epsilonPair.first;
        double eps = epsilonPair.second;
        alphas.insert({ key, -24 * eps });
    }
    // Insert all combinations of betas & gammas into the map -> to use in LJ force
    for (auto sigmaPair : sigmas) {
        std::pair<int, int> key = sigmaPair.first;
        double sig = sigmaPair.second;
        betas.insert({ key, std::pow(sig, 6) });
        gammas.insert({ key, -2 * std::pow(sig, 12) });
    }

    // Warn user about particles outside the domain
    for (auto& particle : container.particles)
        if (cellGrid.determineCellType(cellGrid.getIndexFromPos(particle.getX())) == CellType::Halo)
            spdlog::warn(
                "Generated particle at [{}, {}, {}] outside domain and inside a halo cell",
                particle.getX()[0],
                particle.getX()[1],
                particle.getX()[2]);

    // Warn user about halo cells that contain particles in the beginning
    for (Position pos : allPositions)
        for (auto haloCellIndex : cellGrid.haloCellIterator(pos))
            if (!cellGrid.cells.at(haloCellIndex[0])
                     .at(haloCellIndex[1])
                     .at(haloCellIndex[2])
                     ->getParticles()
                     .empty())
                spdlog::warn(
                    "Halo cell at [{}, {}, {}] contains a particle",
                    haloCellIndex[0],
                    haloCellIndex[1],
                    haloCellIndex[2]);

    // Generate the distances needed for repulsion to occur
    for (auto params : LJParams) {
        if (repulsiveDistances.find(params.first) == repulsiveDistances.end())
            repulsiveDistances.insert({ params.first, params.second.second * std::pow(2, 1 / 6) });
    }

    if (bcHandler.dimensionality != cellGrid.gridDimensionality) {
        spdlog::error(
            "Dimensionality mismatch between boundary conditions and cell grid.",
            "Boundary conditions: {}, Cell grid: {}",
            bcHandler.dimensionality,
            cellGrid.gridDimensionality);
        exit(EXIT_FAILURE);
    }

    // Initialize thermostat
    spdlog::info(
        "Initializing thermostat with T_init={}K, T_target={}K, delta_T={}K with a frequency of {}",
        T_init,
        T_target,
        delta_T,
        n_thermostat);
    themostat = Thermostat(T_init, T_target, delta_T, cellGrid.gridDimensionality);

    // Intialize temperature
    spdlog::info("Setting initial temperature to {} K", T_init);
    themostat.initializeBrownianMotion(this->container);
}

void MixedLJSimulation::runSim()
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
        if (iteration % n_thermostat == 0) {
            themostat.updateT(this->container);
        }
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}

std::pair<int, int> MixedLJSimulation::getMixKey(unsigned type1, unsigned type2)
{
    if (type1 > type2) {
        std::swap(type1, type2);
    }
    // Always return the pair with the smaller type first
    return std::make_pair(type1, type2);
}

double MixedLJSimulation::getRepulsiveDistance(int type) const
{
    spdlog::trace("Got repulsive distance from Mixed LJ sim");
    return repulsiveDistances.at(type);
};
