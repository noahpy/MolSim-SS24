
#include "io/fileReader/asciiReader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>

void AsciiReader::readFile(Simulation& sim)
{
    std::cout << "Ascii!!" << std::endl;
    std::ifstream input_file(filename);
    std::string line;

    if (!input_file.is_open()) {
        spdlog::error("Error: could not open file {}", filename);
        exit(-1);
    }

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
    double vx, vy, vz, mass, spacing;
    if (!std::getline(input_file, line)) {
        spdlog::error("Could not read velocity!");
        exit(EXIT_FAILURE);
    }
    std::istringstream parameter_stream(line);
    if (!(parameter_stream >> vx >> vy >> vz >> mass >> spacing)) {
        spdlog::error("Could not read velocity, mass, and spacing!");
        exit(EXIT_FAILURE);
    }
    std::cout << spacing << std::endl;

    // Read characters representing particles
    std::vector<char> particle_chars;
    std::string particle_chars_line;
    if (!std::getline(input_file, particle_chars_line)) {
        spdlog::error("Could not read particle characters!");
        exit(EXIT_FAILURE);
    }
    std::cout << particle_chars_line << std::endl;
    std::istringstream particle_chars_stream(particle_chars_line);
    char c;
    while (particle_chars_stream >> c) {
        particle_chars.push_back(c);
    }

    // Read ASCII art
    std::vector<std::string> ascii_art;
    while (std::getline(input_file, line)) {
        if (!line.empty()) {
            ascii_art.push_back(line);
        }
    }

    // Generate particles based on ASCII art
    for (size_t i = 0; i < ascii_art.size(); ++i) {
        const std::string& row = ascii_art[i];
        std::cout << row << std::endl;
        for (size_t j = 0; j < row.size(); ++j) {
            char particle_char = row[j];
            // Check if character represents a particle
            if (std::find(particle_chars.begin(), particle_chars.end(), particle_char) !=
                particle_chars.end()) {
                double x = ox + j * spacing; // Calculate particle position
                double y = oy - i * spacing; // Invert y-axis for ASCII art convention
                double z = oz; // Assume particles are in the same plane
                sim.container.addParticle({ { x, y, z }, { vx, vy, vz }, mass });
            }
        }
    }
}
