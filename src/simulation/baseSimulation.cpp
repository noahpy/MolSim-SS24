
#include "simulation/baseSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"

Simulation::Simulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    FileWriter& writer,
    FileReader& reader)
    : time(time)
    , delta_t(delta_t)
    , end_time(end_time)
    , container(container)
    , strategy(strat)
    , writer(writer)
    , reader(reader)
{
    // read simulation parameter from file
    reader.readFile(*this);
}
