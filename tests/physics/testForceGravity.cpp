#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "io/fileReader/FileReader.h"
#include "io/argparse/argparse.h"
#include "io/fileWriter/VTKWriter.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include <chrono>
#include <gtest/gtest.h>
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
            : file("../input/eingabe-sonne.txt"),
              start_time(0),
              end_time(1000),
              delta_t(0.014),
              fileReader(file),
              writer(),
              stratA{location_stroemer_verlet, velocity_stroemer_verlet, force_gravity},
              stratB{location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2},
              particlesA{{}},
              particlesB{{}},
              simA(start_time, delta_t, end_time, particlesA, stratA, writer, fileReader),
              simB(start_time, delta_t, end_time, particlesB, stratB, writer, fileReader)
    {
    }

};

TEST_F(calcForceTest, CompareNaiiveV2) {
    while (simA.time < simA.end_time) {
        // do one step of the simulation
        force_gravity(simA);
        force_gravity_V2(simB);

        for (size_t i = 0; i < simA.container.particles.size(); i++) {
            Particle &pA = simA.container.particles[i];
            Particle &pB = simB.container.particles[i];
            for (size_t j = 0; j < 3; j++) {
                // check if every component is the same
                ASSERT_NEAR(pA.getF()[j], pB.getF()[j], 1e-6);
            }
        }

        simA.time += simA.delta_t;
    }
}

TEST_F(calcForceTest, Timing) {
    int num_iters = 10e6;

    const auto startNaive = std::chrono::high_resolution_clock::now();
    for (int i = 0; i<num_iters; i++) {
        force_gravity(simA);
        simA.time += simA.delta_t;
    }
    const auto endNaive = std::chrono::high_resolution_clock::now();

    const auto startV2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i<num_iters; i++) {
        force_gravity_V2(simB);
        simB.time += simB.delta_t;
    }
    const auto endV2 = std::chrono::high_resolution_clock::now();

    const auto int_ms_naive = std::chrono::duration_cast<std::chrono::milliseconds>(endNaive - startNaive);
    const auto int_ms_v2 = std::chrono::duration_cast<std::chrono::milliseconds>(endV2 - startV2);

    std::cout << "Naive: " << int_ms_naive.count() << "ms" << std::endl;
    std::cout << "V2: " << int_ms_v2.count() << "ms" << std::endl;
    std::cout << "Speedup: " << (double) int_ms_naive.count() / int_ms_v2.count() << "x" << std::endl;
    ASSERT_LT(int_ms_v2.count(), int_ms_naive.count());
}
