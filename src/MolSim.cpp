
#include "io/argparse/argparse.h"
#include "io/fileReader/readerFactory.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/ParticleContainer.h"
#include "physics/stratFactory.h"
#include "physics/strategy.h"
#include "simulation/planetSim.h"
#include "simulation/simFactory.h"
#include "spdlog/spdlog.h"

#include <string>

// Main function
int main(int argc, char* argsv[])
{
    constexpr double start_time = 0; // start time
    double end_time = 0.014 * 10 * 20; // end time
    double delta_t = 0.014; // time increment
    double epsilon = 5; // depth of potential well
    double sigma = 1; // zero-crossing of LJ potential
    std::string input_file; // output filename
    unsigned reader_type = 0;
    unsigned writer_type = 0;
    unsigned simulation_type = 0;

    // parse arguments
    argparse(
        argc,
        argsv,
        end_time,
        delta_t,
        epsilon,
        sigma,
        input_file,
        reader_type,
        writer_type,
        simulation_type);

    // Initialize reader
    auto readPointer = readerFactory(input_file, reader_type);

    // Initialize writer
    auto writePointer = std::make_unique<outputWriter::VTKWriter>();

    // Intialize physics strategy
    PhysicsStrategy strat = stratFactory(simulation_type);

    // Intialize empty particle container
    ParticleContainer particles { {} };

    // Intialize simulation and read the input files
    auto simPointer = simFactory(
        simulation_type,
        start_time,
        delta_t,
        end_time,
        epsilon,
        sigma,
        particles,
        strat,
        std::move(writePointer),
        std::move(readPointer));

    // Run simulation
    simPointer->runSim();

    // inform user that output has been written
    spdlog::info("Output written. Terminating...");
    return 0;
}
