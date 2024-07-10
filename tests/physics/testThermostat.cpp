
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/thermostat/Thermostat.h"
#include "physics/stratFactory.h"
#include <cmath>
#include <gtest/gtest.h>
#include <simulation/planetSim.h>
#include <utils/Params.h>

// Check if heating of temperature works
TEST(thermostatTest, updateTemp)
{
    Particle p1 { { 0, 0, 0 }, { 1, 1, 0 }, 1 };
    Particle p2 { { 0, 0, 0 }, { 0, 1, 1 }, 1 };
    Particle p3 { { 0, 0, 0 }, { 1, 0, 1 }, 1 };

    std::array<double, 3> p1V { sqrt(2), sqrt(2), 0};
    std::array<double, 3> p2V { 0, sqrt(2), sqrt(2)};
    std::array<double, 3> p3V { sqrt(2), 0, sqrt(2) };
    std::array<std::array<double, 3>, 3> expectedVs = { p1V, p2V, p3V };

    Params params {};
    params.start_time = 0;
    params.delta_t = 0;
    params.end_time = 0;
    params.reader_type = ReaderType::EMPTY;
    params.writer_type = WriterType::VTK;
    ParticleContainer container {std::vector<Particle>{p1, p2, p3}};
    auto strat = stratFactory(DOMAIN_LJ);
    auto readPointer = std::make_unique<EmptyFileReader>(params.input_file);
    auto writePointer = std::make_unique<outputWriter::VTKWriter>(params.output_file);

    PlanetSimulation sim(
        params.start_time,
        params.delta_t,
        params.end_time,
        container,
        strat,
        std::move(writePointer),
        std::move(readPointer));

    Thermostat thermo( 1, 2, 1, 2 );
    thermo.updateT(sim.container);

    unsigned pCount = 0;
    for (auto& p : container) {
        EXPECT_EQ(p.getV(), expectedVs.at(pCount));
        ++pCount;
    }
}

// Check if cooling of temperature works
TEST(thermostatTest, updateTempNeg)
{
    Particle p1 { { 0, 0, 0 }, { sqrt(2), sqrt(2), 0 }, 1 };
    Particle p2 { { 0, 0, 0 }, { 0, sqrt(2), sqrt(2) }, 1 };
    Particle p3 { { 0, 0, 0 }, { sqrt(2), 0, sqrt(2) }, 1 };

    std::array<double, 3> p1V { 1, 1, 0};
    std::array<double, 3> p2V { 0, 1, 1};
    std::array<double, 3> p3V { 1, 0, 1 };
    std::array<std::array<double, 3>, 3> expectedVs = { p1V, p2V, p3V };

    Params params {};
    params.start_time = 0;
    params.delta_t = 0;
    params.end_time = 0;
    params.reader_type = ReaderType::EMPTY;
    params.writer_type = WriterType::VTK;
    ParticleContainer container {std::vector<Particle>{p1, p2, p3}};
    auto strat = stratFactory(DOMAIN_LJ);
    auto readPointer = std::make_unique<EmptyFileReader>(params.input_file);
    auto writePointer = std::make_unique<outputWriter::VTKWriter>(params.output_file);

    PlanetSimulation sim(
        params.start_time,
        params.delta_t,
        params.end_time,
        container,
        strat,
        std::move(writePointer),
        std::move(readPointer));

    Thermostat thermo( 2, 1, 1, 2 );
    thermo.updateT(sim.container);

    double PRESICION = 10e-5;
    unsigned pCount = 0;
    for (auto& p : container) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getV().at(i), expectedVs.at(pCount).at(i), PRESICION);
        }
        ++pCount;
    }
}

// Check if keeping temperature works
TEST(thermostatTest, keepTemp)
{
    Particle p1 { { 0, 0, 0 }, { 1, 1, 0 }, 1 };
    Particle p2 { { 0, 0, 0 }, { 0, 1, 1 }, 1 };
    Particle p3 { { 0, 0, 0 }, { 1, 0, 1 }, 1 };

    std::array<double, 3> p1V { 1, 1, 0};
    std::array<double, 3> p2V { 0, 1, 1};
    std::array<double, 3> p3V { 1, 0, 1 };
    std::array<std::array<double, 3>, 3> expectedVs = { p1V, p2V, p3V };

    Params params {};
    params.start_time = 0;
    params.delta_t = 0;
    params.end_time = 0;
    params.reader_type = ReaderType::EMPTY;
    params.writer_type = WriterType::VTK;
    ParticleContainer container {std::vector<Particle>{p1, p2, p3}};
    auto strat = stratFactory(DOMAIN_LJ);
    auto readPointer = std::make_unique<EmptyFileReader>(params.input_file);
    auto writePointer = std::make_unique<outputWriter::VTKWriter>(params.output_file);

    PlanetSimulation sim(
        params.start_time,
        params.delta_t,
        params.end_time,
        container,
        strat,
        std::move(writePointer),
        std::move(readPointer));

    Thermostat thermo( 1, 1, 1, 2 );
    thermo.updateT(sim.container);

    unsigned pCount = 0;
    for (auto& p : container) {
        EXPECT_EQ(p.getV(), expectedVs.at(pCount));
        ++pCount;
    }

    pCount = 0;
    for (auto& p : sim.container) {
        p.setV({sqrt(2), sqrt(2), 0});
        expectedVs.at(pCount) = {1, 1, 0};
        ++pCount;
    }
    thermo.updateT(sim.container);

    double PRESICION = 10e-5;
    pCount = 0;
    for (auto& p : container) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getV().at(i), expectedVs.at(pCount).at(i), PRESICION);
        }
        ++pCount;
    }

    for (auto& p : sim.container) {
        p.setV({0.5, 0.5, 0});
    }
    thermo.updateT(sim.container);

    pCount = 0;
    for (auto& p : container) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_EQ(p.getV().at(i), expectedVs.at(pCount).at(i));
        }
        ++pCount;
    }
}
