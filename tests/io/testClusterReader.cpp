
#include "io/fileReader/clusterReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

class clusterReaderTest : public ::testing::Test {
protected:
    std::string commentFile = "../tests/data/clusterReader1.txt";
    std::string unknownClusterFile = "../tests/data/clusterReader2.txt";
    std::string missingParamFile = "../tests/data/clusterReader3.txt";
    std::string missingTypeFile = "../tests/data/clusterReader4.txt";
    std::string clusterFile = "../tests/data/clusterReader5.txt";
    std::string nonExistFile = "../tests/data/noFile.txt";
    double start_time;
    double end_time;
    double delta_t;
    std::unique_ptr<ClusterReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    ParticleContainer particles;

    clusterReaderTest()
        : start_time(0)
        , end_time(0)
        , delta_t(0.014)
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , particles { {} }
    {
    }
};

// test if no particles are constructed, with an unempty file
TEST_F(clusterReaderTest, testComments)
{
    fileReader = std::make_unique<ClusterReader>(commentFile);
    PlanetSimulation sim {
        start_time, delta_t, end_time, particles, strat, std::move(writer), std::move(fileReader)
    };

    EXPECT_TRUE(particles.particles.empty());
}

// test if program terminates when faced with unknown cluster
TEST_F(clusterReaderTest, testUnknownCluster)
{
    fileReader = std::make_unique<ClusterReader>(unknownClusterFile);
    EXPECT_DEATH(
        {
            PlanetSimulation sim = PlanetSimulation(
                start_time,
                delta_t,
                end_time,
                particles,
                strat,
                std::move(writer),
                std::move(fileReader));
        },
        ".*");
}

// test if program terminates when faced with missing parameter
TEST_F(clusterReaderTest, testMissingParam)
{
    fileReader = std::make_unique<ClusterReader>(missingParamFile);
    EXPECT_DEATH(
        {
            PlanetSimulation sim = PlanetSimulation(
                start_time,
                delta_t,
                end_time,
                particles,
                strat,
                std::move(writer),
                std::move(fileReader));
        },
        ".*");
}

// test if program terminates when faced with missing file
TEST_F(clusterReaderTest, testMissingType)
{
    fileReader = std::make_unique<ClusterReader>(nonExistFile);
    EXPECT_DEATH(
        {
            PlanetSimulation sim = PlanetSimulation(
                start_time,
                delta_t,
                end_time,
                particles,
                strat,
                std::move(writer),
                std::move(fileReader));
        },
        ".*");
}

// test if program generates the particles right
TEST_F(clusterReaderTest, testClusterGeneration)
{
    fileReader = std::make_unique<ClusterReader>(clusterFile);
    PlanetSimulation sim = PlanetSimulation(
        start_time, delta_t, end_time, particles, strat, std::move(writer), std::move(fileReader));

    EXPECT_EQ(384, particles.particles.size());

    for (auto p : particles.particles) {
        EXPECT_EQ(p.getM(), 1);
    }
}
