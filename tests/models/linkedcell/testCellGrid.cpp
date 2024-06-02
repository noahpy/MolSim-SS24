
#include "models/ParticleContainer.h"
#include "models/linked_cell/cell/Cell.h"
#include "models/linked_cell/CellGrid.h"
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

// Test Get Neighboring Particles (Comprehensive)
TEST_F(CellGridTest, GetNeighboringParticles)
{
    for (Particle& p : particles) {
        grid.addParticle(p);
    }


    // 1 from {0, 0, 0}, 1 from {1, 1, 2}
    EXPECT_EQ(grid.getNeighboringParticles({ 1, 1, 1 }).size(), 2);
    // 2 from {2, 2, 3}
    EXPECT_EQ(grid.getNeighboringParticles({ 1, 1, 2 }).size(), 2);
    // 1 from {3, 3, 3}, 1 from {5, 5, 5}
    EXPECT_EQ(grid.getNeighboringParticles({ 4, 4, 4 }).size(), 2);
    // 1 from {5, 5, 5}
    EXPECT_EQ(grid.getNeighboringParticles({ 5, 4, 4 }).size(), 1);
    
    EXPECT_EQ(grid.getNeighboringParticles({ 0, 0, 0 }).size(), 0);

    // Test out-of-bounds index
    EXPECT_THROW({ auto _ = grid.getNeighboringParticles({ 6, 2, 3 }); }, std::out_of_range);
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
