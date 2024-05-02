#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "io/fileReader/FileReader.h"
#include "io/argparse/argparse.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/ParticleContainer.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include <gtest/gtest.h>
#include <vector>

TEST(ForceCalcTest, CompareNaiiveV2)
{
    std::string file = "../../input/eingabe-sonne.txt";
    constexpr double start_time = 0; // start time
    double end_time = 1000; // end time
    double delta_t = 0.014; // time increment

    // Initialize reader
    FileReader fileReader(file);
    // Initialize writer
    outputWriter::VTKWriter writer;

    // Intialize physics strategy
    PhysicsStrategy stratA { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_stroemer_verlet };

    PhysicsStrategy stratB { location_stroemer_verlet,
                             velocity_stroemer_verlet,
                             force_stroemer_verlet_V2 };

    // Intialize empty particle containers
    ParticleContainer particlesA { {} };
    ParticleContainer particlesB { {} };

    // Setup simulation
    PlanetSimulation simA { start_time, delta_t, end_time, particlesA, stratA, writer, fileReader };
    PlanetSimulation simB { start_time, delta_t, end_time, particlesB, stratB, writer, fileReader };

    while (simA.time < simA.end_time)
    {
        // do one step of the simulation
        force_stroemer_verlet(simA);
        force_stroemer_verlet_V2(simB);

        for (size_t i = 0; i < simA.container.particles.size(); i++)
        {
            Particle& pA = simA.container.particles[i];
            Particle& pB = simB.container.particles[i];
            for (size_t j = 0; j < 3; j++)
            {
                // check if every component is the same
                ASSERT_NEAR(pA.getF()[j], pB.getF()[j], 1e-6);
            }
        }

        simA.time += simA.delta_t;
    }
}