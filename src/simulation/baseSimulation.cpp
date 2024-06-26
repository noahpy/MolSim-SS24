
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
    std::unique_ptr<FileReader> reader,
    unsigned frequency)
    : time(time)
    , delta_t(delta_t)
    , end_time(end_time)
    , frequency(frequency)
    , container(container)
    , strategy(strat)
    , writer(std::move(writer))
    , reader(std::move(reader))
{
}

void Simulation::setOutputFile(std::string output)
{
    this->writer->out_name = output;
}
