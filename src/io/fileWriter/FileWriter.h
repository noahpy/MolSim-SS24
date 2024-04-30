#pragma once

#include "simulation/baseSimulation.h"
#include <string>

// Abstract class FileWriter
class FileWriter {
public:
    virtual void plotParticles(const Simulation& s) = 0;

    std::string out_name = "MD_vtk";
};
