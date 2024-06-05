
#pragma once
#include "physics/boundaryConditions/BoundaryConfig.h"
#include <array>
#include <string>

enum ReaderType { STANDARD, CLUSTER, EMPTY, ASCII, XML };

enum WriterType { XYZ, VTK };

enum SimulationType { PLANET, LJ, LINKED_LJ, DOMAIN_LJ };

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
    std::array<double, 3> domain_origin = { -50, -50, 1 };
    // domain size
    std::array<double, 3> domain_size = { 100, 100, 1 };
    // cutoff
    double cutoff = 5;
    // plot frequency
    unsigned plot_frequency = 10;
    // update frequency
    unsigned update_frequency = 10;
    // boundary configuration
    BoundaryConfig boundaryConfig { BoundaryType::OVERFLOW, BoundaryType::OVERFLOW,
                                    BoundaryType::OVERFLOW, BoundaryType::OVERFLOW,
                                    BoundaryType::OVERFLOW, BoundaryType::OVERFLOW };
};
