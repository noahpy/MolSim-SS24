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

class XYZWriter : public FileWriter {
public:
    XYZWriter();

    virtual ~XYZWriter();

    void plotParticles(ParticleContainer particles, const std::string& filename, int iteration);

    void plotParticles(const Simulation& s) override;
};

} // namespace outputWriter
