
#include "simulation/baseSimulation.h"

#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include <utility>

Simulation::Simulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    std::map<unsigned , bool> stationaryParticleTypes,
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
    , progressLogger(time, end_time, delta_t)
{
}

void Simulation::setOutputFile(std::string output)
{
    this->writer->out_name = std::move(output);
}

std::string Simulation::getOutputFile() const
{
    return this->writer->out_name;
}
