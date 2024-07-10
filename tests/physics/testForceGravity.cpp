#include "io/argparse/argparse.h"
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

class calcForceTest : public ::testing::Test {
protected:
    std::string file;
    double start_time;
    double end_time;
    double delta_t;
    FileReader fileReader;
    outputWriter::VTKWriter writer;
    PhysicsStrategy stratA;
    PhysicsStrategy stratB;
    ParticleContainer particlesA;
    ParticleContainer particlesB;
    PlanetSimulation simA;
    PlanetSimulation simB;

    calcForceTest()
        : file("../tests/data/eingabe-sonne.txt")
        , start_time(0)
        , end_time(1000)
        , delta_t(0.014)
        , fileReader(file)
        , writer()
        , stratA { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity }
        , stratB { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2 }
        , particlesA { {} }
        , particlesB { {} }
        , simA(
              start_time,
              delta_t,
              end_time,
              particlesA,
              stratA,
              std::make_unique<outputWriter::VTKWriter>(writer),
              std::make_unique<FileReader>(fileReader))
        , simB(
              start_time,
              delta_t,
              end_time,
              particlesB,
              stratB,
              std::make_unique<outputWriter::VTKWriter>(writer),
              std::make_unique<FileReader>(fileReader))

    {
    }
};

// Compare force caluculation between the two functions
TEST_F(calcForceTest, CompareNaiiveV2)
{
    while (simA.time < simA.end_time) {
        // do one step of the simulation
        force_gravity(simA);
        force_gravity_V2(simB);

        for (size_t i = 0; i < simA.container.particles.size(); i++) {
            for (size_t j = 0; j < 3; j++) {
                // check if every component is the same
                ASSERT_NEAR(simA.container.particles[i].getF()[j], simB.container.particles[i].getF()[j], 1e-6);
            }
        }

        simA.time += simA.delta_t;
    }
}
