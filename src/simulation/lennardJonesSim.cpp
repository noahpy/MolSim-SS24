
#include "lennardJonesSim.h"
#include "physics/strategy.h"
#include "io/fileWriter/FileWriter.h"
#include <cmath>
#include <spdlog/spdlog.h>

LennardJonesSimulation::LennardJonesSimulation(double time,
                                               double delta_t,
                                               double end_time,
                                               ParticleContainer &container,
                                               PhysicsStrategy &strat,
                                               FileWriter &writer,
                                               FileReader &reader,
                                               double epsilon,
                                               double sigma) : Simulation(time, delta_t, end_time, container, strat, writer, reader),
                                               epsilon(epsilon),
                                               sigma(sigma)
{
    this->alpha = -24 * epsilon;
    this->beta = std::pow(sigma, 6);
    this->gamma = -2 * std::pow(sigma,12);
}

void LennardJonesSimulation::runSim()
{
    while (time < end_time) {
        strategy.calF(*this);
        strategy.calV(*this);
        strategy.calX(*this);

        ++iteration;
        if (iteration % 10 == 0) {
            writer.plotParticles(*this);
        }
        spdlog::debug("Iteration {} finished.", iteration);

        time += delta_t;
    }
}
