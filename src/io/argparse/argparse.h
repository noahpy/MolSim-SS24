
#include <string>

/**
 * @brief Parse the command line arguments
 * @param argc Number of arguments
 * @param argsv Command line arguments
 * @param end_time The end time of the simulation
 * @param delta_t The time increment of the simulation
 * @param epsilon The depth of potential well
 * @param sigma The zero-crossing of LJ potential
 * @param input The input file name to load the data from
 * @param reader_type Type specification of the FileReader
 * @param writer_type Type specification of the FileWriter
 * @param simulation_type Type specification of the Simulation
 * @return void, will set the values of end_time, delta_t, and input
 */
void argparse(
    int argc,
    char* argsv[],
    double& end_time,
    double& delta_t,
    double& epsilon,
    double& sigma,
    std::string& input,
    unsigned& reader_type,
    unsigned& writer_type,
    unsigned& simulation_type);
