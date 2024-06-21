
#include "io/fileReader/asciiReader.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <algorithm>
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>

void AsciiReader::readFile(Simulation& sim)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        spdlog::error("Error: could not open file {}", filename);
        exit(EXIT_FAILURE);
    }
    while (readAscii(sim, input_file))
        ;
}

bool AsciiReader::readAscii(Simulation& sim, std::ifstream& input_file)
{
    bool nextArt = false;
    std::string line;

    // Read origin position
    double ox, oy, oz;
    if (!std::getline(input_file, line)) {
        spdlog::error("Could not read origin position!");
        exit(EXIT_FAILURE);
    }
    std::istringstream origin_stream(line);
    if (!(origin_stream >> ox >> oy >> oz)) {
        spdlog::error("Could not parse origin position!");
        exit(EXIT_FAILURE);
    }

    // Read velocity
    double vx, vy, vz, mass, spacing, meanVel;
    if (!std::getline(input_file, line)) {
        spdlog::error("Could not read parameters!");
        exit(EXIT_FAILURE);
    }
    std::istringstream parameter_stream(line);
    if (!(parameter_stream >> vx >> vy >> vz >> mass >> spacing >> meanVel)) {
        spdlog::error("Could not parse velocity, mass, spacing and meanVel!");
        exit(EXIT_FAILURE);
    }

    // Read optional type
    int type = 0;
    parameter_stream >> type;

    // Read characters representing particles
    std::vector<char> particle_chars;
    std::string particle_chars_line;
    if (!std::getline(input_file, particle_chars_line)) {
        spdlog::error("Could not read particle characters!");
        exit(EXIT_FAILURE);
    }
    std::istringstream particle_chars_stream(particle_chars_line);
    char c;
    while (particle_chars_stream >> c) {
        particle_chars.push_back(c);
    }

    spdlog::info(
        "Read Ascii cluster: Origin ({}, {}, {}); Velocity ({}, {}, {}); Mass({}); Particles({}); "
        "Spacing({}); meanVel({}); type({})",
        ox,
        oy,
        oz,
        vx,
        vy,
        vz,
        mass,
        particle_chars_line,
        spacing,
        meanVel,
        type);

    // Read ASCII art
    std::vector<std::string> ascii_art;
    while (std::getline(input_file, line)) {
        if (!line.empty()) {
            ascii_art.push_back(line);
        } else {
            nextArt = true;
            break;
        }
    }

    // Generate particles based on ASCII art
    for (size_t i = 0; i < ascii_art.size(); ++i) {
        const std::string& row = ascii_art[i];
        for (size_t j = 0; j < row.size(); ++j) {
            char particle_char = row[j];
            // Check if character represents a particle
            if (std::find(particle_chars.begin(), particle_chars.end(), particle_char) !=
                particle_chars.end()) {
                double x = ox + j * spacing; // Calculate particle position
                double y = oy - i * spacing; // Invert y-axis for ASCII art convention
                double z = oz; // Assume particles are in the same plane
                std::array<double, 3> initialVel { vx, vy, vz };
                std::array<double, 3> velocity =
                    initialVel + maxwellBoltzmannDistributedVelocity(meanVel, 2);
                sim.container.addParticle({ { x, y, z }, velocity, mass, type });
            }
        }
    }
    return nextArt;
}
