
#include <gtest/gtest.h>
#include "models/linked_cell/Cell.h"
#include "models/linked_cell/BoundaryCell.h"
#include "models/linked_cell/HaloCell.h"
#include "models/linked_cell/InnerCell.h"
#include <vector>

// Check if all cell types can be instantiated
TEST(CellTests, CellTypeInstantiation)
{
    Cell innerCell { CellType::Inner };
    Cell boundaryCell { CellType::Boundary };
    Cell haloCell { CellType::Halo };
    Cell defaultCell {};

    EXPECT_EQ(innerCell.getType(), CellType::Inner);
    EXPECT_EQ(boundaryCell.getType(), CellType::Boundary);
    EXPECT_EQ(haloCell.getType(), CellType::Halo);
    EXPECT_EQ(defaultCell.getType(), CellType::Inner);
}

// Check if subclasses can be instantiated
TEST(CellTests, CellSubclassInstantiation)
{
    InnerCell innerCell {};
    BoundaryCell boundaryCell {std::array<size_t, 3> {0, 0, 0}};
    HaloCell haloCell {std::array<size_t, 3> {0, 0, 0}};
    Cell defaultCell {};

    EXPECT_EQ(innerCell.getType(), CellType::Inner);
    EXPECT_EQ(boundaryCell.getType(), CellType::Boundary);
    EXPECT_EQ(haloCell.getType(), CellType::Halo);
    EXPECT_EQ(defaultCell.getType(), CellType::Inner);
}

// Check if get and set counter works for all classes
TEST(CellTests, CellCounter)
{
    InnerCell innerCell {};
    BoundaryCell boundaryCell {std::array<size_t, 3> {0, 0, 0}};
    HaloCell haloCell {std::array<size_t, 3> {0, 0, 0}};
    Cell defaultCell {};

    innerCell.setCounter(1);
    boundaryCell.setCounter(2);
    haloCell.setCounter(3);
    defaultCell.setCounter(4);

    EXPECT_EQ(innerCell.getCounter(), 1);
    EXPECT_EQ(boundaryCell.getCounter(), 2);
    EXPECT_EQ(haloCell.getCounter(), 3);
    EXPECT_EQ(defaultCell.getCounter(), 4);

    innerCell.setCounter(5);
    boundaryCell.setCounter(6);
    haloCell.setCounter(7);
    defaultCell.setCounter(8);

    EXPECT_EQ(innerCell.getCounter(), 5);
    EXPECT_EQ(boundaryCell.getCounter(), 6);
    EXPECT_EQ(haloCell.getCounter(), 7);
    EXPECT_EQ(defaultCell.getCounter(), 8);
}

// Check if particles can be added and removed
TEST(SimpleCellParticleTest, AddRemoveParticles)
{
    InnerCell innerCell {};
    BoundaryCell boundaryCell {std::array<size_t, 3> {0, 0, 0}};
    HaloCell haloCell {std::array<size_t, 3> {0, 0, 0}};
    Cell defaultCell {};
    std::vector<Cell*> cells;
    cells.push_back(&innerCell);
    cells.push_back(&boundaryCell);
    cells.push_back(&haloCell);
    cells.push_back(&defaultCell);

    Particle p1 {std::array<double, 3> {0, 0, 0}, std::array<double, 3> {0, 0, 0}, 1, 0};
    Particle p2 {std::array<double, 3> {0, 0, 0}, std::array<double, 3> {0, 0, 0}, 2, 0};
    Particle p3 {std::array<double, 3> {0, 0, 0}, std::array<double, 3> {0, 0, 0}, 3, 0};

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
    InnerCell innerCell {};
    BoundaryCell boundaryCell { std::array<size_t, 3> { 0, 0, 0 } };
    HaloCell haloCell { std::array<size_t, 3> { 0, 0, 0 } };
    Cell defaultCell {};
    std::vector<Cell*> cells;
    cells.push_back(&innerCell);
    cells.push_back(&boundaryCell);
    cells.push_back(&haloCell);
    cells.push_back(&defaultCell);

    Particle p1 {std::array<double, 3> {1, 2, 3}, std::array<double, 3> {-1, -2, -3}, 1, 0};
    Particle p2 {std::array<double, 3> {2, 3, 4}, std::array<double, 3> {-2, -3, -4}, 2, 0};
    Particle p3 {std::array<double, 3> {3, 4, 5}, std::array<double, 3> {-3, -4, -5}, 3, 0};

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
                EXPECT_EQ(particle.get().getV()[index], -1 -index - count);
            }
            EXPECT_EQ(particle.get().getM(), count + 1);
            count++;

            cCount++;
        }
        EXPECT_EQ(cCount, 3);
    }
}