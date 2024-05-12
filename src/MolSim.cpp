
#include "io/argparse/argparse.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/ParticleContainer.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include "spdlog/spdlog.h"

#include "models/generators/ParticleGenerator.h"

#include <string>

// Main function
int main(int argc, char* argsv[])
{
    constexpr double start_time = 0; // start time
    double end_time = 0.014 * 10 * 20; // end time
    double delta_t = 0.014; // time increment
    std::string input_file; // output filename

    // parse arguments
    argparse(argc, argsv, end_time, delta_t, input_file);

    // Initialize reader
    FileReader fileReader(input_file);
    // Initialize writer
    outputWriter::VTKWriter writer;

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones };

    // Intialize empty particle container
    ParticleContainer particles { {} };

    // Setup simulation
    PlanetSimulation sim { start_time, delta_t, end_time, particles, strat, writer, fileReader };
    LennardJonesSimulation simLJ { start_time, delta_t, end_time, particles, strat, writer, fileReader , 5, 1};

    CuboidParticleCluster cluster1 = CuboidParticleCluster(std::array<double, 3> {0,0,0}, 40,8,1, 1.1225, 1, std::array<double, 3> {0,0,0}, 0.1, 2);
    CuboidParticleCluster cluster2 = CuboidParticleCluster(std::array<double, 3> {15,15,0}, 8,8,1, 1.1225, 1, std::array<double, 3> {0,-10,0}, 0.1, 2);

    ParticleGenerator p = ParticleGenerator(simLJ.container);
    p.registerCluster(std::make_unique<CuboidParticleCluster>(cluster1));
    p.registerCluster(std::make_unique<CuboidParticleCluster>(cluster2));
    p.generateClusters();

    // Run simulation
    // sim.runSim();
    simLJ.runSim();

    // inform user that output has been written
    spdlog::info("Output written. Terminating...");
    return 0;
}
