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

class FileReader {
public:
    FileReader(std::string filename);
    virtual ~FileReader();

    void readFile(ParticleContainer& particles, std::string filename);

    void readFile(Simulation& sim);

private:
    std::string filename;
};
