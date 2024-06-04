
#include "linkedLennardJonesSim.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include <spdlog/spdlog.h>

LinkedLennardJonesSimulation::LinkedLennardJonesSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writer,
    std::unique_ptr<FileReader> reader,
    double epsilon,
    double sigma,
    std::array<double, 3> domainOrigin,
    std::array<double, 3> domainSize,
    double cutoff,
    unsigned frequency,
    unsigned updateFrequency,
    bool read_file)
    : LennardJonesSimulation(
          time,
          delta_t,
          end_time,
          container,
          strat,
          std::move(writer),
          std::move(reader),
          epsilon,
          sigma,
          frequency,
          false)
    , cellGrid(domainOrigin, domainSize, cutoff)
    , updateFrequency(updateFrequency)
{
    if (read_file) {
        this->reader->readFile(*this);
    }
    cellGrid.addParticlesFromContainer(container);
}

void LinkedLennardJonesSimulation::runSim()
{
    while (time < end_time) {
        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        ++iteration;
        if (iteration % frequency == 0) {
            writer->plotParticles(*this);
        }
        if (iteration % updateFrequency == 0) {
            cellGrid.updateCells();
        }
        spdlog::trace("Iteration {} finished.", iteration);

        time += delta_t;
    }
}

void LinkedLennardJonesSimulation::setDomainOrigin(const std::array<double, 3>& domainOrigin)
{
    cellGrid.setDomainOrigin(domainOrigin);
}

void LinkedLennardJonesSimulation::setDomainSize(const std::array<double, 3>& domainSize)
{
    cellGrid.setDomainSize(domainSize);
}

void LinkedLennardJonesSimulation::setCutoff(double cutoff)
{
    cellGrid.setCutoffRadius(cutoff);
}
