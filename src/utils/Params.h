
#pragma once
#include "physics/boundaryConditions/BoundaryConfig.h"
#include <array>
#include <string>

enum ReaderType { STANDARD, CLUSTER, EMPTY, ASCII, XML };

enum class WriterType { XYZ, VTK, XML, EMPTY };

enum SimulationType { PLANET, LJ, LINKED_LJ, DOMAIN_LJ, MIXED_LJ };

class Params {
public:
    // start time
    double start_time = 0;
    // end time
    double end_time = 0.0005 * 10 * 20;
    // time increment
    double delta_t = 0.0005;
    // depth of potential well
    double epsilon = 5;
    // zero-crossing of LJ potential
    double sigma = 1;
    // input filename
    std::string input_file;
    // output filename
    std::string output_file = "sim";
    // reader type
    ReaderType reader_type = ReaderType::CLUSTER;
    // writer type
    WriterType writer_type = WriterType::VTK;
    // simulation type
    SimulationType simulation_type = SimulationType::MIXED_LJ;
    // domain origin
    std::array<double, 3> domain_origin = { 2, 0, 0 };
    // domain size
    std::array<double, 3> domain_size = { 25.2, 30, 12 };
    // cutoff
    double cutoff = 2.75;
    // plot frequency
    unsigned plot_frequency = 10;
    // update frequency
    unsigned update_frequency = 10;
    // checkpoint frequency
    unsigned checkpoint_frequency = 1000000;
    // boundary configuration
    BoundaryConfig boundaryConfig { BoundaryType::WALL,     BoundaryType::WALL,
                                    BoundaryType::PERIODIC, BoundaryType::PERIODIC,
                                    BoundaryType::PERIODIC, BoundaryType::PERIODIC };
    // initial temperature
    double init_temp = 40;
    // target temperature
    double target_temp = 40;
    // frequency of thermostat updates
    unsigned thermo_freq = 10;
    // maximum temperature delta
    double max_temp_delta = 10000;
    // gravitational constant
    double gravity = -0.8;
    // particle types
    std::vector<std::pair<double, double>> particleTypes { { 2.0, 1.1 },
                                                           { 2.0, 1.1 },
                                                           { 1.0, 1.0 } };
    // map to particle types
    std::map<unsigned, std::pair<double, double>> typesMap { { 2, { 2.0, 1.1 } },
                                                             { 1, { 2.0, 1.1 } },
                                                             {0, { 1.0, 1.0 }} };
    // Flag for measuring performance -> will not use any io and time the simulation
    bool doPerformanceMeasurements = false;
};
