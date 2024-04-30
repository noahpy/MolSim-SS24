

#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>

void printHelp(std::string progName)
{
    std::cout << "Usage: " << progName << " [OPTIONS] FILE" << std::endl
              << "Simulate particle dynamics and output results in FILE in VTK format." << std::endl
              << "Options:" << std::endl
              << "  -d, --delta_t=VALUE    Set the time step (default: 0.014)" << std::endl
              << "  -e, --end_time=VALUE   Set the end time for simulation (default: 0.28)"
              << std::endl
              << "  -h, --help             Display this help message" << std::endl;
}

void argparse(int argc, char* argsv[], double& end_time, double& delta_t, std::string& output)
{
    // Long options definition
    static struct option long_options[] = { { "delta_t", required_argument, 0, 'd' },
                                            { "end_time", required_argument, 0, 'e' },
                                            { "help", no_argument, 0, 'h' },
                                            { 0, 0, 0, 0 } };

    int opt;
    while ((opt = getopt_long(argc, argsv, "d:e:h", long_options, NULL)) != -1) {
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

    output = argsv[optind];
}
