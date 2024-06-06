
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/ParticleContainer.h"
#include "models/linked_cell/cell/Cell.h"
#include "physics/stratFactory.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include <array>
#include <gtest/gtest.h>
#include <vector>

// Test if the soft reflective Boundary calculates the right position for the particles used to
// reflect the particle in a boundary cell. Compare these coordinates with hand calculated
// coordinates
TEST(SoftReflectiveBoundary, CorrectMirroring)
{
    spdlog::set_level(spdlog::level::off);

    std::array<double, 3> domain_origin = { -10, -10, -10 };
    Params simParams {};
    simParams.end_time = simParams.delta_t * 0.5;
    simParams.reader_type = ReaderType::EMPTY;
    simParams.writer_type = WriterType::VTK;
    simParams.simulation_type = SimulationType::DOMAIN_LJ;
    simParams.domain_origin = domain_origin;
    simParams.domain_size = { 5, 5, 5 };

    std::array<double, 3> pos = { -9, -8, -7 };
    ParticleContainer container { std::vector<Particle> { Particle { pos, { 0, 0, 0 }, 1 } } };

    std::map<Position, std::array<double, 3>> expectedMirroredParticles {
        { LEFT, { -11, -8, -7 } },   { RIGHT, { -1, -8, -7 } }, { TOP, { -9, -2, -7 } },
        { BOTTOM, { -9, -12, -7 } }, { FRONT, { -9, -8, -3 } }, { BACK, { -9, -8, -13 } }
    };

    auto strat = stratFactory(DOMAIN_LJ);
    auto readPointer = std::make_unique<EmptyFileReader>(simParams.input_file);
    auto writePointer = std::make_unique<outputWriter::VTKWriter>(simParams.output_file);

    LennardJonesDomainSimulation LJDSim(
        simParams.start_time,
        simParams.delta_t,
        simParams.end_time,
        container,
        strat,
        std::move(writePointer),
        std::move(readPointer),
        simParams.epsilon,
        simParams.sigma,
        simParams.domain_origin,
        simParams.domain_size,
        simParams.cutoff,
        simParams.boundaryConfig,
        simParams.plot_frequency,
        simParams.update_frequency);

    // Check if particle is at the right spot and cell
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(LJDSim.container.particles[0].getX()[i], pos[i]);
    }
    EXPECT_EQ(LJDSim.getGrid().cells[1][1][1]->getParticles().size(), 1);

    // ----- Do sim step
    LJDSim.bcHandler.preUpdateBoundaryHandling(LJDSim);
    strat.calF(LJDSim);
    strat.calV(LJDSim);
    strat.calX(LJDSim);
    // -----

    // Check the mirrored particles
    for (auto position : allPositions) {
        for (auto haloCellIndex : LJDSim.getGrid().haloCellIterator(position)) {
            for (auto& particle : LJDSim.getGrid()
                                      .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                                      ->getParticles()) {
                for (int i = 0; i < 3; i++) {
                    EXPECT_NEAR(
                        particle.get().getX()[i], expectedMirroredParticles[position][i], 1e-8);
                }
            }
        }
    }
}
