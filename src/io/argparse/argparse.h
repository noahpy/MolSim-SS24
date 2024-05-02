
#include <string>

/**
 * @brief Parse the command line arguments
 * @param argc Number of arguments
 * @param argsv Command line arguments
 * @param end_time The end time of the simulation
 * @param delta_t The time increment of the simulation
 * @param output The output file name
 * @return void, will set the values of end_time, delta_t, and output
 */
void argparse(int argc, char* argsv[], double& end_time, double& delta_t, std::string& output);
