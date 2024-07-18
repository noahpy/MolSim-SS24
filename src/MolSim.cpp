
#include "io/argparse/argparse.h"
#include "io/fileReader/readerFactory.h"
#include "io/fileWriter/writerFactory.h"
#include "io/xmlparse/xmlparse.h"
#include "models/ParticleContainer.h"
#include "physics/stratFactory.h"
#include "physics/strategy.h"
#include "physics/thermostat/ThermostatFactory.h"
#include "simulation/planetSim.h"
#include "simulation/simFactory.h"
#include "spdlog/spdlog.h"
#include "utils/Params.h"
#include <string>

// Main function
int main(int argc, char* argsv[])
{
    Params params;

    // parse arguments
    argparse(argc, argsv, params);

    // optionally parse xml
    if (params.reader_type == ReaderType::XML) {
        xmlparse(params, params.input_file);
    }

    // Initialize reader
    auto readPointer = readerFactory(params.input_file, params.reader_type);

    // Initialize writer
    auto writePointer = writerFactory(params.writer_type, params.output_file);

    // Initialize thermostat
    auto thermostat = thermostatFactory(
        params.thermostat_type,
        params.init_temp,
        params.target_temp,
        params.max_temp_delta,
        params.domain_size[2] > 1 ? 3 : 2);

    // Intialize physics strategy
    PhysicsStrategy strat = stratFactory(params.simulation_type);

    // Intialize empty particle container
    ParticleContainer particles {};

    // Intialize simulation and read the input files
    auto simPointer = simFactory(
        params,
        particles,
        strat,
        std::move(writePointer),
        std::move(readPointer),
        std::move(thermostat));

    // Run simulation
    simPointer->runSim();

    // inform user that output has been written
    spdlog::info("Output written. Terminating...");
    return 0;
}
