/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "ParticleContainer.h"


namespace outputWriter {

class XYZWriter {
public:
    XYZWriter();

    virtual ~XYZWriter();

    void plotParticles(ParticleContainer particles, const std::string& filename, int iteration);
};

} // namespace outputWriter
