

#include "utils/Params.h"
#include <getopt.h>
#include <iostream>
#include <spdlog/spdlog.h>
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
              << "Read particle information from FILE, simulate particle dynamics and output "
                 "results in VTK format"
              << std::endl
              << "Options:" << std::endl
              << "  -d, --delta_t=VALUE    Set the time step (default: 0.014)" << std::endl
              << "  -e, --end_time=VALUE   Set the end time for simulation (default: 2.8)"
              << std::endl
              << "      --epsilon=VALUE    Set the depth of LJ potential well (default: 5)"
              << std::endl
              << "      --sigma=VALUE      Set the Zero crossing of LJ potential (default: 1)"
              << std::endl
              << "  -l, --log_level=LEVEL  Set the logging level (default: 3)" << std::endl
              << "  -c                     Specify that the given input file describes clusters "
              << std::endl
              << "  -a                     Specify that the given input file is of type ascii art"
              << std::endl
              << "  -x                     Specify that the given input file is of type XML"
              << std::endl
              << "  -s, --simtype=VALUE    Specify simulation type (default: 0)" << std::endl
              << "  -w, --writetype=VALUE  Specify writer type (default: 0)" << std::endl
              << "  -h, --help             Display this help message" << std::endl
              << std::endl
              << "For more details, see the man page with: man ./.molsim.1" << std::endl;
}

/**
 * @brief For setting spdlog level with default level info
 * @param level Integers 1 to 7 from parameter argument corresponds to level trace to off
 * @return
 */
spdlog::level::level_enum getLogLevel(int level)
{
    switch (level) {
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

void convertToInt(char* optarg, int& value)
{
    try {
        value = std::stoi(optarg);
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid int argument: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    } catch (std::out_of_range& e) {
        std::cout << "Given int is out of range: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    }
}

void convertToUnsigned(char* optarg, unsigned& value)
{
    try {
        value = std::stoul(optarg);
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid int argument: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    } catch (std::out_of_range& e) {
        std::cout << "Given int is out of range: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    }
}

void convertToDouble(char* optarg, double& value)
{
    try {
        value = std::stod(optarg);
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid float argument: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    } catch (std::out_of_range& e) {
        std::cout << "Given float is out of range: " << optarg << std::endl;
        exit(EXIT_FAILURE);
    }
}

void argparse(
    int argc,
    char* argsv[],
    Params& params)
{
    // Long options definition
    static struct option long_options[] = { { "delta_t", required_argument, 0, 'd' },
                                            { "end_time", required_argument, 0, 'e' },
                                            { "log_level", required_argument, 0, 'l' },
                                            { "epsilon", required_argument, 0, 'E' },
                                            { "sigma", required_argument, 0, 'S' },
                                            { "simtype", required_argument, 0, 's' },
                                            { "writetype", required_argument, 0, 'w' },
                                            { "help", no_argument, 0, 'h' },
                                            { 0, 0, 0, 0 } };

    int opt;
    while ((opt = getopt_long(argc, argsv, "d:e:l:hcs:w:ax", long_options, NULL)) != -1) {
        switch (opt) {
        case 'd':
            convertToDouble(optarg, params.delta_t);
            break;
        case 'e':
            convertToDouble(optarg, params.end_time);
            break;
        case 'l':
            int loglevel;
            convertToInt(optarg, loglevel);
            spdlog::set_level(getLogLevel(loglevel));
            break;
        case 'E':
            convertToDouble(optarg, params.epsilon);
            break;
        case 'S':
            convertToDouble(optarg, params.sigma);
            break;
        case 's':
            convertToUnsigned(optarg, params.simulation_type);
            break;
        case 'w':
            convertToUnsigned(optarg, params.writer_type);
            break;
        case 'c':
            params.reader_type = 1;
            break;
        case 'a':
            params.reader_type = 3;
            break;
        case 'x':
            params.reader_type = 4;
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

    params.input_file = argsv[optind];
}
