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

    /**
     * @brief Constructor of FileWriter
     */
    FileWriter() = default;

    /** @brief Constructor of FileWriter */
    inline FileWriter(std::string out_name)
        : out_name(out_name)
    {
    }

    /**
     * @brief The name of the output file
     */
    std::string out_name = "MD_vtk";

    /*
     * @brief Destructor of FileWriter
     * */
    virtual ~FileWriter() = default;
};
