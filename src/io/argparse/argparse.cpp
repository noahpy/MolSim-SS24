

#include "spdlog/spdlog.h"

#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Print the help message
 * @param progName The name of the program
 * @return void
 */
void printHelp(std::string progName)
{
    std::cout << "Usage: " << progName << " [OPTIONS] FILE" << std::endl
              << "Simulate particle dynamics and output results in FILE in VTK format." << std::endl
              << "Options:" << std::endl
              << "  -d, --delta_t=VALUE    Set the time step (default: 0.014)" << std::endl
              << "  -e, --end_time=VALUE   Set the end time for simulation (default: 0.28)"
              << std::endl
              << "  -l, --log_level=LEVEL  Set the logging level (1=trace, 2=debug, 3=info, 4=warn, 5=err, 6=critical, 7=off; default: 3)" << std::endl
              << "  -h, --help             Display this help message" << std::endl;
}

/**
 * @brief For setting spdlog level with default level info
 * @param level Integers 1 to 7 from parameter argument corresponds to level trace to off
 * @return
 */
spdlog::level::level_enum getLogLevel(int level)
{
    switch(level){
        case 1:
            return spdlog::level::trace;
        case 2:
            return spdlog::level::debug;
        case 3:
            return spdlog::level::info;
        case 4:
            return spdlog::level::warn;
        case 5:
            return spdlog::level::err;
        case 6:
            return spdlog::level::critical;
        case 7: 
            return spdlog::level::off;
    }
    return spdlog::level::info;
}

void argparse(int argc, char* argsv[], double& end_time, double& delta_t, std::string& input)
{
    // Long options definition
    static struct option long_options[] = { { "delta_t", required_argument, 0, 'd' },
                                            { "end_time", required_argument, 0, 'e' },
                                            {"log_level", required_argument, 0, 'l'},
                                            { "help", no_argument, 0, 'h' },
                                            { 0, 0, 0, 0 } };

    int opt;
    while ((opt = getopt_long(argc, argsv, "d:e:l:h", long_options, NULL)) != -1) {
        switch (opt) {
        case 'd':
            try {
                delta_t = std::stof(optarg);
            } catch (std::invalid_argument& e) {
                std::cout << "Invalid float argument: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            } catch (std::out_of_range& e) {
                std::cout << "Given float is out of range: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case 'e':
            try {
                end_time = std::stof(optarg);
            } catch (std::invalid_argument& e) {
                std::cout << "Invalid float argument: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            } catch (std::out_of_range& e) {
                std::cout << "Given float is out of range: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case 'l':
            try {
                spdlog::set_level(getLogLevel(std::stoi(optarg)));
            } catch (std::invalid_argument& e) {
                std::cout << "Invalid int argument: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            } catch (std::out_of_range& e) {
                std::cout << "Given int is out of range: " << optarg << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
            printHelp(argsv[0]);
            exit(EXIT_SUCCESS);
        case '?':
        default:
            printHelp(argsv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (argc - optind != 1) {
        std::cout << "Missing positional argument: FILE" << std::endl;
        printHelp(argsv[0]);
        exit(EXIT_FAILURE);
    }

    input = argsv[optind];
}
