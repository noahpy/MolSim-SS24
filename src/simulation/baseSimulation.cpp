
#include "simulation/baseSimulation.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"

Simulation::Simulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader)
    : time(time)
    , delta_t(delta_t)
    , end_time(end_time)
    , container(container)
    , strategy(strat)
    , writer(std::move(writer))
    , reader(std::move(reader))
{
    // read simulation parameter from file
    this->reader->readFile(*this);
}
