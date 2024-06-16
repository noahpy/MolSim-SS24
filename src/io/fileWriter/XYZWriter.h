/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "io/fileWriter/FileWriter.h"
#include "models/ParticleContainer.h"

namespace outputWriter {

/**
 * @brief Class XYZWriter to write the simulation data to a file in XYZ format
 */
class XYZWriter : public FileWriter {
public:
    /**
     * @brief Constructor for the XYZWriter class
     * @return XYZWriter object
     */
    XYZWriter();

    /*
     * @brief Initializes the XYZWriter class
     * @param out_name the name of the output file
     */
    XYZWriter(std::string out_name)
        : FileWriter(out_name)
    {
    }

    /**
     * @brief Destructor for the XYZWriter class
     * @return void
     */
    virtual ~XYZWriter();

    /**
     * @brief Write the simulation data for all particles to a file for one iteration
     *
     * @param particles ParticleContainer object containing the particles to be written
     * @param filename Name of the file to be written
     * @param iteration Iteration number of the simulation
     *
     * @return void
     */
    void plotParticles(ParticleContainer particles, const std::string& filename, int iteration);

    /**
     * @brief Write the simulation data to a file using the simulation object
     *
     * @param s Simulation object
     *
     * @return void
     */
    void plotParticles(const Simulation& s) override;
};

} // namespace outputWriter
