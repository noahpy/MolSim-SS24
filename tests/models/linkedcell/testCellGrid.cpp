
#include "models/ParticleContainer.h"
#include "models/linked_cell/CellGrid.h"
#include "models/linked_cell/cell/Cell.h"
#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

class CellGridTest : public ::testing::Test {
protected:
    std::array<double, 3> domainOrigin { 0.0, 0.0, 0.0 };
    std::array<double, 3> domainSize { 10.0, 10.0, 10.0 };
    // This will be increased to 2.5 internally
    double cutoffRadius = 2.3;
    CellGrid grid { domainOrigin, domainSize, cutoffRadius };

    std::vector<Particle> particles {
        Particle({ 1.0, 2.0, 3.0 }, { 0.1, 0.2, 0.3 }, 1.0), // Boundary
        Particle({ 0, 0, 0 }, { 0.1, 0.1, 0.1 }, 1.0), // Boundary
        Particle({ 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }, 2.0), // Boundary
        Particle({ 9.9, 9.9, 9.9 }, { 1.0, 1.0, 1.0 }, 1.5), // Boundary
        Particle({ 3.5, 3.14159, 7 }, { 0.1, 0.1, 0.1 }, 1.0), // Inner
        Particle({ 3.5, 3.14159, 7 }, { 0.1, 0.1, 0.1 }, 1.0), // Inner
        Particle({ 5, 5, 5 }, { 0.1, 0.1, 0.1 }, 1.0), // Inner
        Particle({ -0.1, -0.1, -0.1 }, { 1.0, 1.0, 1.0 }, 1.5), // Halo
        Particle({ 1000, 0, 0 }, { 1.0, 1.0, 1.0 }, 1.5), // Halo
        Particle({ 10, 10, 10 }, { 0.1, 0.1, 0.1 }, 1.0), // Halo
    };

    CellGridTest()
    {
        for (auto& p : particles) {
            p.setF({ 3, 4, 0.1 });
        }
    }
};

// Test Initialization
TEST_F(CellGridTest, Initialization)
{
    // Ensure the correct number of cells are initialized
    EXPECT_EQ(grid.cells.size(), 6);
    for (size_t i = 0; i < grid.cells.size(); ++i) {
        EXPECT_EQ(grid.cells[i].size(), 6);
        for (size_t j = 0; j < grid.cells[i].size(); ++j) {
            EXPECT_EQ(grid.cells[i][j].size(), 6);
        }
    }
}

// Test Particle Addition (Single)
TEST_F(CellGridTest, AddSingleParticle)
{
    for (Particle& p : particles) {
        grid.addParticle(p);
    }

    // Boundary Cells
    EXPECT_EQ(grid.cells[1][1][2]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[1][1][2]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[1][1][1]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[1][1][1]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[4][4][4]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[4][4][4]->getType(), CellType::Boundary);

    // Inner Cells
    EXPECT_EQ(grid.cells[2][2][2]->getParticles().size(), 0);
    EXPECT_EQ(grid.cells[2][2][2]->getType(), CellType::Inner);

    EXPECT_EQ(grid.cells[2][2][3]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[2][2][3]->getType(), CellType::Inner);

    EXPECT_EQ(grid.cells[3][3][3]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[3][3][3]->getType(), CellType::Inner);

    // Halo Cells
    EXPECT_EQ(grid.cells[0][0][0]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[0][0][0]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][1][1]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][1][1]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][5][5]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][5][5]->getType(), CellType::Halo);
}

// Test Particle Addition (from Container)
TEST_F(CellGridTest, AddParticlesFromContainer)
{
    ParticleContainer container(particles);

    grid.addParticlesFromContainer(container);

    // Boundary Cells
    EXPECT_EQ(grid.cells[1][1][2]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[1][1][2]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[1][1][1]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[1][1][1]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[4][4][4]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[4][4][4]->getType(), CellType::Boundary);

    // Inner Cells
    EXPECT_EQ(grid.cells[2][2][3]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[2][2][3]->getType(), CellType::Inner);

    EXPECT_EQ(grid.cells[3][3][3]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[3][3][3]->getType(), CellType::Inner);

    // Halo Cells
    EXPECT_EQ(grid.cells[0][0][0]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[0][0][0]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][1][1]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][1][1]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][5][5]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][5][5]->getType(), CellType::Halo);
}

// Test CellGrid Update
TEST_F(CellGridTest, CellGridUpdateTest)
{
    ParticleContainer container(particles);

    grid.addParticlesFromContainer(container);

    // Boundary Cells
    EXPECT_EQ(grid.cells[1][1][2]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[1][1][2]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[1][1][1]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[1][1][1]->getType(), CellType::Boundary);

    EXPECT_EQ(grid.cells[4][4][4]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[4][4][4]->getType(), CellType::Boundary);

    // Inner Cells
    EXPECT_EQ(grid.cells[2][2][3]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[2][2][3]->getType(), CellType::Inner);

    EXPECT_EQ(grid.cells[3][3][3]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[3][3][3]->getType(), CellType::Inner);

    // Halo Cells
    EXPECT_EQ(grid.cells[0][0][0]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[0][0][0]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][1][1]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][1][1]->getType(), CellType::Halo);

    EXPECT_EQ(grid.cells[5][5][5]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[5][5][5]->getType(), CellType::Halo);

    // Update particle position
    for (Particle& p : container) {
        p.setX({ p.getX().at(0) + 0.5, p.getX().at(1) + 0.5, p.getX().at(2) + 0.5 });
    }

    grid.updateCells();

    // Check update cell
    EXPECT_EQ(grid.cells[0][0][0]->getParticles().size(), 0);
    EXPECT_EQ(grid.cells[1][1][1]->getParticles().size(), 3);
    EXPECT_EQ(grid.cells[1][1][2]->getParticles().size(), 0);
    EXPECT_EQ(grid.cells[2][2][3]->getParticles().size(), 0);
    EXPECT_EQ(grid.cells[2][2][4]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[3][3][3]->getParticles().size(), 1);
    EXPECT_EQ(grid.cells[4][4][4]->getParticles().size(), 0);
    EXPECT_EQ(grid.cells[5][5][5]->getParticles().size(), 2);
    EXPECT_EQ(grid.cells[5][1][1]->getParticles().size(), 1);
}

// sum the size of neighbouring cells
int sumNeighboringCells(CellGrid& grid, std::list<CellIndex>& indices)
{
    int sum = 0;
    for (CellIndex& i : indices) {
        sum += grid.cells.at(i.at(0)).at(i.at(1)).at(i.at(2))->getParticles().size();
    }
    return sum;
}

// Test Get Neighboring Particles
TEST_F(CellGridTest, GetNeighboringParticles)
{
    for (Particle& p : particles) {
        grid.addParticle(p);
    }

    // 1 from {0, 0, 0}, 1 from {1, 1, 2}
    auto indices = grid.getNeighbourCells({ 1, 1, 1 });
    EXPECT_EQ(indices.size(), 26);
    EXPECT_EQ(sumNeighboringCells(grid, indices), 2);
    EXPECT_EQ(grid.cells.at(1).at(1).at(1)->getCounter(), 7);
    // check effetcs on neighbour
    EXPECT_TRUE(grid.cells.at(1).at(1).at(1)->visited);
    for (auto parRef : grid.cells.at(1).at(1).at(1)->getParticles()) {
        for (auto& f : parRef.get().getF()) {
            EXPECT_EQ(f, 0.0);
        }
        for (auto& f : parRef.get().getOldF()) {
            EXPECT_NE(f, 0.0);
        }
    }

    // 2 from {2, 2, 3}
    indices = grid.getNeighbourCells({ 2, 2, 3 });
    EXPECT_EQ(indices.size(), 26);
    EXPECT_EQ(sumNeighboringCells(grid, indices), 2);
    EXPECT_EQ(grid.cells.at(2).at(2).at(3)->getCounter(), 26);

    // 1 from {3, 3, 3}, 1 from {5, 5, 5}
    indices = grid.getNeighbourCells({ 4, 4, 4 });
    EXPECT_EQ(indices.size(), 26);
    EXPECT_EQ(sumNeighboringCells(grid, indices), 2);
    EXPECT_EQ(grid.cells.at(4).at(4).at(4)->getCounter(), 7);

    // 1 from {5, 5, 5} , but is a halo cell*/
    indices = grid.getNeighbourCells({ 5, 4, 4 });
    EXPECT_EQ(indices.size(), 0);

    // Also halo cell
    EXPECT_EQ(grid.getNeighbourCells({ 0, 0, 0 }).size(), 0);

    // Test out-of-bounds index
    EXPECT_THROW({ auto _ = grid.getNeighbourCells({ 6, 2, 3 }); }, std::out_of_range);
}

// Test 5: Boundary and Halo Iterators
TEST_F(CellGridTest, BoundaryAndHaloIterators)
{
    std::map<CellIndex, CellType> cellMap;

    // Count boundary particles
    size_t boundaryCount = 0;
    for (auto it = grid.beginBoundaryParticles(); it != grid.endBoundaryParticles(); ++it) {
        ++boundaryCount;
        // check if cell is boundary
        CellIndex i = *it;
        EXPECT_EQ(grid.cells.at(i[0]).at(i[1]).at(i[2])->getType(), CellType::Boundary);
        // check if cell is unique
        EXPECT_EQ(cellMap.count(i), 0);
        cellMap[i] = CellType::Boundary;
    }

    EXPECT_EQ(boundaryCount, 56);

    // Halo cells have no particles, but the test should still be able to run
    size_t haloCount = 0;
    for (auto it = grid.beginHaloParticles(); it != grid.endHaloParticles(); ++it) {
        ++haloCount;
        // check if cell is halo
        CellIndex i = *it;
        EXPECT_EQ(grid.cells.at(i[0]).at(i[1]).at(i[2])->getType(), CellType::Halo);
        // check if cell is unique
        EXPECT_EQ(cellMap.count(i), 0);
        cellMap[i] = CellType::Halo;
    }

    EXPECT_EQ(haloCount, 152);
}
