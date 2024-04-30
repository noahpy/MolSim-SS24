
#include "models/ParticleContainer.h"
#include "simulation/baseSimulation.h"

class StandardSimulation : public Simulation {
public:

    using Simulation::Simulation;

    StandardSimulation(
    double time,
    double delta_t,
    double end_time,
    ParticleContainer& container,
    PhysicsStrategy& strat,
    FileWriter& writer,
    FileReader& reader);

    void runSim() override;
};
