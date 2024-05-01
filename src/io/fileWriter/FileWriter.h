#pragma once

#include "simulation/baseSimulation.h"
#include <string>

/**
 * @brief Abstract class FileWriter
 */
class FileWriter {
public:
    /**
     * @brief Write the simulation data to a file
     * @param s Simulation object
     */
    virtual void plotParticles(const Simulation& s) = 0;

    std::string out_name = "MD_vtk";
};
