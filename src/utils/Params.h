
#pragma once
#include <array>
#include <string>

class Params {
public:
    double start_time = 0; // start time
    double end_time = 0.014 * 10 * 20; // end time
    double delta_t = 0.014; // time increment
    double epsilon = 5; // depth of potential well
    double sigma = 1; // zero-crossing of LJ potential
    std::string input_file; // input filename
    std::string output_file = "sim";
    unsigned reader_type = 0;
    unsigned writer_type = 0;
    unsigned simulation_type = 0;
    std::array<double, 3> domain_origin = { -50, -50, 0 };
    std::array<double, 3> domain_size = { 100, 100, 1 };
    double cutoff = 5;
    unsigned plot_frequency = 10;
    unsigned update_frequency = 10;
};
