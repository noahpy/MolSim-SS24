
#include "io/fileWriter/FileWriter.h"

/* @brief FileWriter that does nothing */
class EmptyFileWriter : public FileWriter {
    /**
     * @brief Write the simulation data to a file
     * @param s Simulation object
     */
    virtual void plotParticles(const Simulation& s);
};
