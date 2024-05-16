
#include "simulation/planetSim.h"
#include "io/fileWriter/FileWriter.h"
#include "physics/strategy.h"
#include <iostream>

PlanetSimulation::PlanetSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    FileWriter& writer,
    FileReader& reader)
    : Simulation(time, delta_t, end_time, container, strat, writer, reader)
{
}

void PlanetSimulation::runSim()
{
    while (time < end_time) {
        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        ++iteration;
        if (iteration % 10 == 0) {
            writer.plotParticles(*this);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        time += delta_t;
    }
}
