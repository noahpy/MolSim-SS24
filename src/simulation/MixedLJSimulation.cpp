
#include "MixedLJSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include "utils/ArrayUtils.h"

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
    unsigned int n_thermostat,
    bool doProfile)
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
    , ljparams(LJParams)
    , doProfile(doProfile)
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

    if (n_thermostat) {
        // Initialize thermostat
        spdlog::info(
            "Initializing thermostat with T_init={}K, T_target={}K, delta_T={}K with a frequency "
            "of {}",
            T_init,
            T_target,
            delta_T,
            n_thermostat);
        thermostat = Thermostat(T_init, T_target, delta_T, cellGrid.gridDimensionality);

        // Intialize temperature
        spdlog::info("Setting initial temperature to {} K", T_init);
        thermostat.initializeBrownianMotion(this->container);
    }
    else{
        spdlog::info("Themostat is turned off.");
    }
}

void MixedLJSimulation::runSim()
{

    container.particles[874].setType(2);
    container.particles[875].setType(2);
    container.particles[924].setType(2);
    container.particles[925].setType(2);


    auto startTime = std::chrono::steady_clock::now();
    unsigned long long particleUpdates = 0;
    while (time < end_time) {
        bcHandler.preUpdateBoundaryHandling(*this);

        strategy.calF(*this);

        if (time < 150) {
            std::array<double, 3> Fz_up = { 0, 0, 0.8 };
            Particle& p1 = container.particles[874];
            Particle& p2 = container.particles[875];
            Particle& p3 = container.particles[924];
            Particle& p4 = container.particles[925];
            p1.setOldF(p1.getF());
            p1.setF(p1.getF() + Fz_up);
            p2.setOldF(p2.getF());
            p2.setF(p2.getF() + Fz_up);
            p3.setOldF(p3.getF());
            p3.setF(p3.getF() + Fz_up);
            p4.setOldF(p4.getF());
            p4.setF(p4.getF() + Fz_up);
        }

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
        if (doProfile) {
            particleUpdates += container.activeParticleCount;
        }
        // Update thermostat if frequency is positive
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
    auto endTime = std::chrono::steady_clock::now();
    long elapsedTimeInMS =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    long elapsedTimeInS =
        std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    if (doProfile) {
        // Set log level to info, as it was off for the measurements
        spdlog::set_level(spdlog::level::info);
        spdlog::info(
            "Simulation ran for {} seconds ({} iterations)", elapsedTimeInS, iteration - 1);
        spdlog::info("Average time per iteration: {} ms", elapsedTimeInMS / (iteration - 1));
        spdlog::info(
            "MUP/S = {} (MUP = force+vel+pos calc i.e. one update per particle per iteration)",
            particleUpdates / elapsedTimeInS);
    }
}

double MixedLJSimulation::getRepulsiveDistance(int type) const
{
    spdlog::trace("Got repulsive distance from Mixed LJ sim");
    return repulsiveDistances.at(type);
};
