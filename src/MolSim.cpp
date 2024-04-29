
#include "FileReader.h"
#include "ParticleContainer.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticlesXYZ(int iteration);

/**
 * plot the particles to a vtk-file
 */
void plotParticlesVTK(int iteration);

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

constexpr double start_time = 0;
double end_time = 0.014 * 10 * 20;
double delta_t = 0.014;

ParticleContainer particles { {} };

int main(int argc, char* argsv[])
{
    // Long options definition
    static struct option long_options[] = { { "delta_t", required_argument, 0, 'd' },
                                            { "end_time", required_argument, 0, 'e' },
                                            { "help", required_argument, 0, 'h' },
                                            { 0, 0, 0, 0 } };

    int opt;
    while ((opt = getopt_long(argc, argsv, "d:e:h", long_options, NULL)) != -1) {
        switch (opt) {
        case 'd':
            try {
                delta_t = std::stof(optarg);
            } catch (std::invalid_argument& e) {
                std::cout << "Invalid float argument: " << optarg << std::endl;
                return EXIT_FAILURE;
            } catch (std::out_of_range& e) {
                std::cout << "Given float is out of range: " << optarg << std::endl;
                return EXIT_FAILURE;
            }
            break;
        case 'e':
            try {
                end_time = std::stof(optarg);
            } catch (std::invalid_argument& e) {
                std::cout << "Invalid float argument: " << optarg << std::endl;
                return EXIT_FAILURE;
            } catch (std::out_of_range& e) {
                std::cout << "Given float is out of range: " << optarg << std::endl;
                return EXIT_FAILURE;
            }
            break;
        case 'h':
            printHelp(argsv[0]);
            return EXIT_SUCCESS;
        case '?':
        default:
            printHelp(argsv[0]);
            return EXIT_FAILURE;
        }
    }

    if (argc - optind != 1) {
        std::cout << "Missing positional argument: FILE" << std::endl;
        printHelp(argsv[0]);
        return EXIT_FAILURE;
    }

    FileReader fileReader;
    fileReader.readFile(particles, argsv[optind]);

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            plotParticlesVTK(iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF()
{
    for (auto& p1 : particles) {
        std::array<double, 3> f_i = { 0.0, 0.0, 0.0 };
        for (auto& p2 : particles) {
            double m_mul = p1.getM() * p2.getM();
            double dist = std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3);
            // check for distance = 0
            if (dist == 0)
                continue;
            double coeff = m_mul / dist;
            // f_i = f_i + f_ij
            f_i = f_i + coeff * (p2.getX() - p1.getX());
        }
        p1.setOldF(p1.getF());
        p1.setF(f_i);
    }
}

void calculateX()
{
    for (auto& p : particles) {
        // x = x + Δt * v + (Δt)^2 * F / (2 * m)
        p.setX(p.getX() + delta_t * p.getV() + (delta_t * delta_t / (2 * p.getM())) * p.getF());
    }
}

void calculateV()
{
    for (auto& p : particles) {
        // v = v + Δt * (F + F_old) / (2 * m)
        p.setV(p.getV() + (delta_t / (2 * p.getM())) * (p.getOldF() + p.getF()));
    }
}

void plotParticlesXYZ(int iteration)
{
    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}

void plotParticlesVTK(int iteration)
{
    outputWriter::VTKWriter writer;
    writer.initializeOutput(particles.particles.size());

    for (auto& p : particles) {
        writer.plotParticle(p);
    }

    std::string out_name("MD_vtk");
    writer.writeFile(out_name, iteration);
}
