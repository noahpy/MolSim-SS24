/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "models/ParticleContainer.h"
#include "simulation/baseSimulation.h"
#include <string>

/**
 * @brief Class for reading input files for a simulation.
 */
class FileReader {
public:
    /**
     * @brief Construct a new FileReader object
     * @param filename The name of the file to read
     * @return FileReader
     */
    FileReader(std::string filename);
    /**
     * @brief Destroy the FileReader object
     * @return void
     */
    virtual ~FileReader();

    /**
     * @brief Read a file and store the data in a ParticleContainer
     * @param particles The particle container to store the data in
     * @param filename The name of the file to read
     * @return void
     */
    void readFile(ParticleContainer& particles, std::string filename);

    /**
     * @brief Read a file and store the data in a Simulation
     * @param sim The simulation to store the data in
     * @return void
     */
    virtual void readFile(Simulation& sim);



protected:
    /**
     * @brief The name of the file to read
     */
    std::string filename;
};
