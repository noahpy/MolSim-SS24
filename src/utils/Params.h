
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
    double end_time = 0.014 * 10 * 20;
    // time increment
    double delta_t = 0.014;
    // depth of potential well
    double epsilon = 5;
    // zero-crossing of LJ potential
    double sigma = 1;
    // input filename
    std::string input_file;
    // output filename
    std::string output_file = "sim";
    // reader type
    ReaderType reader_type = ReaderType::STANDARD;
    // writer type
    WriterType writer_type = WriterType::VTK;
    // simulation type
    SimulationType simulation_type = SimulationType::PLANET;
    // domain origin
    std::array<double, 3> domain_origin = { -10.0, -10.0, 0 };
    // domain size
    std::array<double, 3> domain_size = { 20.0, 20.0, 0 };
    // cutoff
    double cutoff = 5;
    // plot frequency
    unsigned plot_frequency = 10;
    // update frequency
    unsigned update_frequency = 10;
    // checkpoint frequency
    unsigned checkpoint_frequency = 10;
    // boundary configuration
    BoundaryConfig boundaryConfig { BoundaryType::SOFT_REFLECTIVE,
                                    BoundaryType::SOFT_REFLECTIVE,
                                    BoundaryType::SOFT_REFLECTIVE,
                                    BoundaryType::SOFT_REFLECTIVE };
    // initial temperature
    double init_temp = 0;
    // target temperature
    double target_temp = 0;
    // frequency of thermostat updates
    unsigned thermo_freq = 20;
    // maximum temperature delta
    double max_temp_delta = 10;
    // gravitational constant
    double gravity = 0.0;
    // particle types 
    std::vector<std::pair<double, double>> particleTypes;
    // map to particle types
    std::map<unsigned, std::pair<double, double>> typesMap;
    // Flag for measuring performance -> will not use any io and time the simulation
    bool doPerformanceMeasurements = false;
    // List of all types which should be immobile
    std::map<unsigned , bool> immobileParticleTypes { {2, true} };
};
