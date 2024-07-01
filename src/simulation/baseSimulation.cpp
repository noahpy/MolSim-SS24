
#include "simulation/baseSimulation.h"

#include <utility>
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
    std::vector<unsigned> stationaryParticleTypes,
    unsigned frequency)
    : time(time)
    , delta_t(delta_t)
    , end_time(end_time)
    , frequency(frequency)
    , container(container)
    , strategy(strat)
    , writer(std::move(writer))
    , reader(std::move(reader))
    , stationaryParticleTypes(std::move(stationaryParticleTypes))
{
}

void Simulation::setOutputFile(std::string output)
{
    this->writer->out_name = output;
}

std::string Simulation::getOutputFile() const
{
    return this->writer->out_name;
}
