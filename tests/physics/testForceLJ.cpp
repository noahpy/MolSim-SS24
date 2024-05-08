
#include "io/fileReader/FileReader.h"
#include "io/fileWriter/VTKWriter.h"
#include <gtest/gtest.h>

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
    /* PlanetSimulation simA; */
    /* PlanetSimulation simB; */

    calcForceTest()
            : file("../input/eingabe-sonne.txt"),
              start_time(0),
              end_time(1000),
              delta_t(0.014),
              fileReader(file),
              writer(),
              stratA{location_stroemer_verlet, velocity_stroemer_verlet, force_stroemer_verlet},
              stratB{location_stroemer_verlet, velocity_stroemer_verlet, force_stroemer_verlet_V2},
              particlesA{{}},
              particlesB{{}},
              simA(start_time, delta_t, end_time, particlesA, stratA, writer, fileReader),
              simB(start_time, delta_t, end_time, particlesB, stratB, writer, fileReader)
    {
    }

};
