
#include "models/linked_cell/cell/Cell.h"
#include <gtest/gtest.h>
#include <vector>

// Check if all cell types can be instantiated
TEST(CellTests, CellTypeInstantiation)
{
    Cell innerCell { CellType::Inner, { 0, 0, 0 } };
    Cell boundaryCell { CellType::Boundary, { 0, 0, 0 } };
    Cell haloCell { CellType::Halo, { 0, 0, 0 } };

    EXPECT_EQ(innerCell.getType(), CellType::Inner);
    EXPECT_EQ(boundaryCell.getType(), CellType::Boundary);
    EXPECT_EQ(haloCell.getType(), CellType::Halo);
}

// Check if subclasses can be instantiated
TEST(CellTests, CellSubclassInstantiation)
{
    Cell innerCell { CellType::Inner, { 0, 0, 0 } };
    Cell boundaryCell { CellType::Boundary, std::array<size_t, 3> { 0, 0, 0 } };
    Cell haloCell { CellType::Halo, std::array<size_t, 3> { 0, 0, 0 } };

    EXPECT_EQ(innerCell.getType(), CellType::Inner);
    EXPECT_EQ(boundaryCell.getType(), CellType::Boundary);
    EXPECT_EQ(haloCell.getType(), CellType::Halo);
}

// Check if get and set counter works for all classes
TEST(CellTests, CellCounter)
{
    Cell innerCell { CellType::Inner, { 0, 0, 0 } };
    Cell boundaryCell { CellType::Boundary, std::array<size_t, 3> { 0, 0, 0 } };
    Cell haloCell { CellType::Halo, std::array<size_t, 3> { 0, 0, 0 } };

    innerCell.setCounter(1);
    boundaryCell.setCounter(2);
    haloCell.setCounter(3);

    EXPECT_EQ(innerCell.getCounter(), 1);
    EXPECT_EQ(boundaryCell.getCounter(), 2);
    EXPECT_EQ(haloCell.getCounter(), 3);

    innerCell.setCounter(5);
    boundaryCell.setCounter(6);
    haloCell.setCounter(7);

    EXPECT_EQ(innerCell.getCounter(), 5);
    EXPECT_EQ(boundaryCell.getCounter(), 6);
    EXPECT_EQ(haloCell.getCounter(), 7);
}

// Check if particles can be added and removed
TEST(SimpleCellParticleTest, AddRemoveParticles)
{
    Cell innerCell { CellType::Inner, { 0, 0, 0 } };
    Cell boundaryCell { CellType::Boundary, std::array<size_t, 3> { 0, 0, 0 } };
    Cell haloCell { CellType::Halo, std::array<size_t, 3> { 0, 0, 0 } };

    std::vector<Cell*> cells;
    cells.push_back(&innerCell);
    cells.push_back(&boundaryCell);
    cells.push_back(&haloCell);

    Particle p1 { std::array<double, 3> { 0, 0, 0 }, std::array<double, 3> { 0, 0, 0 }, 1, 0 };
    Particle p2 { std::array<double, 3> { 0, 0, 0 }, std::array<double, 3> { 0, 0, 0 }, 2, 0 };
    Particle p3 { std::array<double, 3> { 0, 0, 0 }, std::array<double, 3> { 0, 0, 0 }, 3, 0 };

    for (auto cell : cells) {
        cell->addParticle(p1);
        cell->addParticle(p2);
        cell->addParticle(p3);
    }

    for (auto cell : cells) {
        auto particles = cell->getParticles();
        EXPECT_EQ(particles.size(), 3);

        cell->removeParticle(p1);

        particles = cell->getParticles();
        EXPECT_EQ(particles.size(), 2);
    }
}

// Check if enhanced for loop over particles works
TEST(SimpleCellParticleTest, ParticleIterate)
{
    Cell innerCell { CellType::Inner, { 0, 0, 0 } };
    Cell boundaryCell { CellType::Boundary, std::array<size_t, 3> { 0, 0, 0 } };
    Cell haloCell { CellType::Halo, std::array<size_t, 3> { 0, 0, 0 } };
    std::vector<Cell*> cells;
    cells.push_back(&innerCell);
    cells.push_back(&boundaryCell);
    cells.push_back(&haloCell);

    Particle p1 { std::array<double, 3> { 1, 2, 3 }, std::array<double, 3> { -1, -2, -3 }, 1, 0 };
    Particle p2 { std::array<double, 3> { 2, 3, 4 }, std::array<double, 3> { -2, -3, -4 }, 2, 0 };
    Particle p3 { std::array<double, 3> { 3, 4, 5 }, std::array<double, 3> { -3, -4, -5 }, 3, 0 };

    for (auto cell : cells) {
        cell->addParticle(p1);
        cell->addParticle(p2);
        cell->addParticle(p3);
    }

    for (auto cell : cells) {
        int cCount = 0;
        int count = 0;
        for (auto& particle : *cell) {
            for (int index = 0; index < 3; index++) {
                EXPECT_EQ(particle.get().getX()[index], 1 + index + count);
                EXPECT_EQ(particle.get().getV()[index], -1 - index - count);
            }
            EXPECT_EQ(particle.get().getM(), count + 1);
            count++;

            cCount++;
        }
        EXPECT_EQ(cCount, 3);
    }
}

std::array<double, 3> zero { 0, 0, 0 };

// Check if unique pair iteration works on the particle reference list in cell
TEST(PairListItTests, pairListIterator)
{

    std::vector<Particle> particles {
        Particle(zero, zero, 5, 0),
        Particle(zero, zero, 3, 0),
        Particle(zero, zero, 1, 0),
        Particle(zero, zero, 9, 0),
        Particle(zero, zero, 8, 0),
    };

    Cell cell(CellType::Inner, {0, 0, 0});

    for (int i = 0; i < 5; ++i) {
        Particle& p_ref = particles[i];
        cell.addParticle(p_ref);
    }

    std::vector<std::pair<Particle&, Particle&>> pairs;

    unsigned count = 0;
    for (auto it = cell.beginPairs(); it != cell.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        for (std::pair<Particle&, Particle&> particle_pair : pairs) {
            Particle pp1 = particle_pair.first;
            Particle pp2 = particle_pair.second;
            EXPECT_FALSE(
                pt_pair.first.getM() == pp1.getM() && pt_pair.second.getM() == pp2.getM() ||
                pt_pair.first.getM() == pp2.getM() && pt_pair.second.getM() == pp1.getM())
                << "Found duplicate pair!";
        }
        pairs.emplace_back(pt_pair);
        ++count;
    }

    EXPECT_EQ(count, 10);
}

// Check if the pair iteration works on empty list and list with one element
TEST(PairListItTests, pairListItZero)
{
    Particle p1 (zero, zero, 5, 0);
    Cell cell(CellType::Inner, {0, 0, 0});
    Particle& p_ref = p1;
    cell.addParticle(p_ref);

    unsigned count = 0;
    for (auto it = cell.beginPairs(); it != cell.endPairs(); ++it) {
        ++count;
    }

    EXPECT_EQ(count, 0);

    cell.removeParticle(p_ref);
    for (auto it = cell.beginPairs(); it != cell.endPairs(); ++it) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}
