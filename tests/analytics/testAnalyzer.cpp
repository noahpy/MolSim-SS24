
#include "analytics/Analyzer.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/thermostat/ThermostatFactory.h"
#include "physics/velocityCal/velocityCal.h"
#include "utils/Params.h"
#include <fstream>
#include <gtest/gtest.h>

class AnalyzerTester : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    std::array<double, 3> domainOrigin = { 0, 0, 0 };
    std::array<double, 3> domainSize = { 10, 10, 10 };
    const double cutoff = 2.5;
    std::map<unsigned, std::pair<double, double>> LJParams { { 0, { 1, 2 } } };
    double gravityConst = 0;
    Particle p1 { { 1, 5, 1 }, { 3, 0, 4 }, 1, 3 };
    Particle p2 { { 7, 5, 1 }, { 9, 0, 0 }, 2, 3 };
    Particle p3 { { 1, 5, 7 }, { 0, 0, 76 }, 3, 3 };
    ParticleContainer particles = std::vector<Particle> { p1, p2, p3 };
    std::string outName = "test_out";
    std::string pathPrefix = "./";

    AnalyzerTester()
        : start_time(0)
        , end_time(0.05)
        , delta_t(0.0005)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
    {
        spdlog::set_level(spdlog::level::off);
    }
};

// Test if analyzer will write output file at all
TEST_F(AnalyzerTester, testOutputExists)
{
    Analyzer analyzer({ 1, 2, 4 }, outName);
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MixedLJSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW),
        std::move(analyzerPtr),
        gravityConst,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        0,
        0,
        1);

    sim.runSim();

    std::ifstream file_density(pathPrefix + outName + "_density.csv");
    // check if it could be found
    EXPECT_TRUE(file_density.good());
    file_density.close();

    std::ifstream file_velocity(pathPrefix + outName + "_velocity.csv");
    // check if it could be found
    EXPECT_TRUE(file_velocity.good());
    file_velocity.close();

    // delete the files
    remove((pathPrefix + outName + "_density.csv").c_str());
    remove((pathPrefix + outName + "_velocity.csv").c_str());
}

// Test if the analyzer will not output anything if there are no bins
TEST_F(AnalyzerTester, testZeroBins)
{
    Analyzer analyzer({ 0, 2, 4 }, outName);
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MixedLJSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW),
        std::move(analyzerPtr),
        gravityConst,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        0,
        0,
        1);

    sim.runSim();

    std::ifstream file(pathPrefix + outName + "_density.csv");
    // check if it could not be found
    EXPECT_FALSE(file.good());
}

// Test if analyzer calculates the velocity profile correctly (against hand calculated values)
TEST_F(AnalyzerTester, testCorrectVelocityProfile)
{
    Analyzer analyzer({ 2, 1, 2 }, outName);
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MixedLJSimulation sim(
        0,
        0.0005,
        0.0005,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW),
        std::move(analyzerPtr),
        gravityConst,
        thermostatFactory(ThermostatType::NONE, 0, 0, 0, 0),
        0,
        0,
        1);

    sim.runSim();

    std::ifstream file(pathPrefix + outName + "_velocity.csv");
    // check if it could not be found
    EXPECT_TRUE(file.good());
    if (!file.good())
        return;

    // read first line
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);

    // split line at ','
    int count = 0;
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        count++;
        if (count == 1)
            EXPECT_EQ(std::stof(cell), 5);
        else if (count == 2)
            EXPECT_EQ(std::stof(cell),9);
        else if (count == 3)
            EXPECT_EQ(std::stof(cell), 76);
        else if (count == 4)
            EXPECT_EQ(std::stof(cell), 0);
        else
            EXPECT_TRUE(false);
    }

    EXPECT_EQ(count, 4);
    file.close();

    // delete the files
    remove((pathPrefix + outName + "_velocity.csv").c_str());
    remove((pathPrefix + outName + "_density.csv").c_str());
}

// Test if analyzer calculates the density profile correctly (against hand calculated values)
TEST_F(AnalyzerTester, testCorrectDensityProfile)
{
    Analyzer analyzer({ 2, 1, 2 }, outName);
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MixedLJSimulation sim(
        0,
        0.0005,
        0.0005,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW),
        std::move(analyzerPtr),
        gravityConst,
        thermostatFactory(ThermostatType::NONE, 0, 0, 0, 0),
        0,
        0,
        1);

    sim.runSim();

    std::ifstream file(pathPrefix + outName + "_density.csv");
    // check if it could not be found
    EXPECT_TRUE(file.good());
    if (!file.good())
        return;

    // read first line
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);

    // split line at ','
    int count = 0;
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        count++;
        if (count == 1)
            EXPECT_EQ(std::stof(cell), 1);
        else if (count == 2)
            EXPECT_EQ(std::stof(cell), 1);
        else if (count == 3)
            EXPECT_EQ(std::stof(cell), 1);
        else if (count == 4)
            EXPECT_EQ(std::stof(cell), 0);
        else
            EXPECT_TRUE(false);
    }

    EXPECT_EQ(count, 4);
    file.close();

    // delete the files
    remove((pathPrefix + outName + "_velocity.csv").c_str());
    remove((pathPrefix + outName + "_density.csv").c_str());
}