/*
 * VTKWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "io/fileWriter/FileWriter.h"
#include "io/fileWriter/vtk-unstructured.h"
#include "models/Particle.h"
#include "simulation/baseSimulation.h"

namespace outputWriter {

/**
 * @brief This class implements the functionality to generate vtk output from particles.
 */
class VTKWriter : public FileWriter {
public:
    /**
     * @brief Constructor of the VTKWriter class
     * @return A new VTKWriter object
     */
    VTKWriter();

    /**
     * @brief Destructor of the VTKWriter class
     * @return void
     */
    virtual ~VTKWriter();

    /**
     * @brief Writes the simulation data to a file for one iteration
     * @param s Simulation object
     */
    void plotParticles(const Simulation& s) override;

    /**
     * @brief Set up internal data structures and prepare to plot a particle
     * @param numParticles The number of particles to be plotted
     * @return void
     */
    void initializeOutput(int numParticles);

    /**
     * @brief plot type, mass, position, velocity and force of a particle.
     * @note: initializeOutput() must have been called before.
     * @param p the particle to be plotted
     * @return void
     */
    void plotParticle(Particle& p);

    /**
     * @brief writes the final output file.
     *
     * @note This will write a file for all particles plotted so far using plotParticle()
     *
     * @param filename the base name of the file to be written.
     * @param iteration the number of the current iteration,
     *        which is used to generate an unique filename
     *
     * @return void
     */
    void writeFile(const std::string& filename, int iteration);

private:
    /**
     * @brief The VTK file object
     */
    VTKFile_t* vtkFile;
};

} // namespace outputWriter
