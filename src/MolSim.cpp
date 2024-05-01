
#include "io/argparse/argparse.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/ParticleContainer.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

int main(int argc, char* argsv[])
{
    constexpr double start_time = 0;
    double end_time = 0.014 * 10 * 20;
    double delta_t = 0.014;
    std::string output_file;

    // parse arguments
    argparse(argc, argsv, end_time, delta_t, output_file);

    // Initialize reader
    FileReader fileReader(output_file);
    // Initialize writer
    outputWriter::VTKWriter writer;

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_stroemer_verlet_V2 };

    // Intialize empty particle container
    ParticleContainer particles { {} };

    // Setup simulation
    PlanetSimulation sim { start_time, delta_t, end_time, particles, strat, writer, fileReader };

    // Run simulation
    sim.runSim();

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}
