
#pragma once

#include "models/ParticleContainer.h"

// forward-declare PhysicsStrategy
class PhysicsStrategy;
// forward-declare FileWriter
class FileWriter;
// forward-declare FileReader
class FileReader;

// Abstract base class
class Simulation {
public:
    Simulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        FileWriter& writer,
        FileReader& reader);

    virtual void runSim() = 0;

    double time;
    double delta_t;
    double end_time;
    unsigned iteration = 0;
    ParticleContainer& container;

protected:
    PhysicsStrategy& strategy;
    FileWriter& writer;
    FileReader& reader;
};
