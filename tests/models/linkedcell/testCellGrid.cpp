
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

    // Ensure right neighbour instantiation

    // Inner inner cells
    for (size_t i = 3; i < grid.cells.size() - 4; ++i) {
        for (size_t j = 3; j < grid.cells[i].size() - 4; ++j) {
            for (size_t k = 3; k < grid.cells[i][j].size() - 4; ++k) {
                EXPECT_EQ(grid.cells[i][j][k]->innerNeighbours.size(), 26);
                EXPECT_EQ(grid.cells[i][j][k]->haloNeighbours.size(), 0);
                EXPECT_EQ(grid.cells[i][j][k]->boundaryNeighbours.size(), 0);
            }
        }
    }
    // Outer inner cells
    for (auto i : std::vector<size_t> { 2, grid.cells.size() - 3 }) {
        for (auto j : std::vector<size_t> { 2, grid.cells[i].size() - 3 }) {
            for (auto k : std::vector<size_t> { 2, grid.cells[i][j].size() - 3 }) {
                EXPECT_LE(grid.cells[i][j][k]->innerNeighbours.size(), 17)
                    << "Position: " << i << " " << j << " " << k;
                EXPECT_GE(grid.cells[i][j][k]->boundaryNeighbours.size(), 9)
                    << "Position: " << i << " " << j << " " << k;
                EXPECT_EQ(grid.cells[i][j][k]->haloNeighbours.size(), 0)
                    << "Position: " << i << " " << j << " " << k;
                // check the sum of inner and boundary
                EXPECT_EQ(
                    grid.cells[i][j][k]->innerNeighbours.size() +
                        grid.cells[i][j][k]->boundaryNeighbours.size(),
                    26);
            }
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

    size_t boundaryCount = 0;
    size_t haloCount = 0;

    for (auto position : allPositions) {
        // Count boundary particles
        for (auto i : grid.boundaryCellIterator(position)) {
            ++boundaryCount;
            // check if cell is boundary
            EXPECT_EQ(grid.cells.at(i[0]).at(i[1]).at(i[2])->getType(), CellType::Boundary);
            // check if cell is unique
            // EXPECT_EQ(cellMap.count(i), 0);
            cellMap[i] = CellType::Boundary;
        }

        // Halo cells have no particles, but the test should still be able to run
        for (auto i : grid.haloCellIterator(position)) {
            ++haloCount;
            // check if cell is halo
            EXPECT_EQ(grid.cells.at(i[0]).at(i[1]).at(i[2])->getType(), CellType::Halo);
            // check if cell is unique
            // EXPECT_EQ(cellMap.count(i), 0);
            cellMap[i] = CellType::Halo;
        }
    }

    EXPECT_EQ(boundaryCount, 4 * 4 * 6);
    EXPECT_EQ(haloCount, 6 * 6 * 6);
}

// Check, if a particle is placed in all cells of a 2d grid, if they are mapped to the right index,
// and type of cell
TEST(CellGridTestDimensionaity, GridInit2D)
{
    std::array<double, 3> domain_origin = { -50, -50, -50 };
    std::array<double, 3> domain_size = { 100, 100, 0 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    std::vector<Particle> particles {};
    std::vector<CellIndex> indices {};

    double x = domain_origin[0] - cutoff / 2;
    size_t indexX = 0, indexY = 0;
    while (x < domain_origin[0] + domain_size[1] + cutoff) {
        double y = domain_origin[1] - cutoff / 2;
        indexY = 0;
        while (y < domain_origin[1] + domain_size[1] + cutoff) {
            particles.push_back(
                Particle({ x, y, domain_origin[2] }, { 0, 0, domain_origin[2] }, 1.0));
            indices.push_back({ indexX, indexY, 0 });
            y += cutoff;
            indexY++;
        }
        x += cutoff;
        indexX++;
    }

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    for (size_t i = 0; i < indices.size(); ++i) {
        CellIndex index = indices[i];
        // only one particle per cell
        EXPECT_EQ(grid.cells.at(index[0]).at(index[1]).at(index[2])->getParticles().size(), 1);
        // It is the one we expect it to be
        for (size_t ind = 0; ind < 3; ind++)
            EXPECT_EQ(
                grid.cells.at(index[0])
                    .at(index[1])
                    .at(index[2])
                    ->getParticles()
                    .front()
                    .get()
                    .getX()[ind],
                particles[i].getX()[ind]);
    }
    // check if all cells have received one particle
    EXPECT_EQ(particles.size(), (domain_size[0] / cutoff + 2) * (domain_size[1] / cutoff + 2));

    // check boundary iterator
    for (auto position : allPositions) {
        for (auto boundaryIndex : grid.boundaryCellIterator(position)) {
            // It is a boundary cell
            EXPECT_EQ(
                grid.cells.at(boundaryIndex[0])
                    .at(boundaryIndex[1])
                    .at(boundaryIndex[2])
                    ->getType(),
                CellType::Boundary);
            // Check if the particle in the cell has the expected position
            for (size_t ind = 0; ind < 2; ind++) {
                double expectedCoordinate =
                    domain_origin[ind] - cutoff / 2 + cutoff * (double)boundaryIndex[ind];
                EXPECT_EQ(
                    grid.cells.at(boundaryIndex[0])
                        .at(boundaryIndex[1])
                        .at(boundaryIndex[2])
                        ->getParticles()
                        .front()
                        .get()
                        .getX()[ind],
                    expectedCoordinate);
            }

            EXPECT_EQ(
                grid.cells.at(boundaryIndex[0])
                    .at(boundaryIndex[1])
                    .at(boundaryIndex[2])
                    ->getParticles()
                    .front()
                    .get()
                    .getX()[2],
                domain_origin[2]);
        }
    }

    // check halo iterator
    for (auto position : allPositions) {
        for (auto haloIndex : grid.haloCellIterator(position)) {
            // It is a halo cell
            EXPECT_EQ(
                grid.cells.at(haloIndex[0]).at(haloIndex[1]).at(haloIndex[2])->getType(),
                CellType::Halo);
            // Check if the particle in the cell has the expected position
            for (size_t ind = 0; ind < 2; ind++) {
                double expectedCoordinate =
                    domain_origin[ind] - cutoff / 2 + cutoff * (double)haloIndex[ind];
                EXPECT_EQ(
                    grid.cells.at(haloIndex[0])
                        .at(haloIndex[1])
                        .at(haloIndex[2])
                        ->getParticles()
                        .front()
                        .get()
                        .getX()[ind],
                    expectedCoordinate);
            }

            EXPECT_EQ(
                grid.cells.at(haloIndex[0])
                    .at(haloIndex[1])
                    .at(haloIndex[2])
                    ->getParticles()
                    .front()
                    .get()
                    .getX()[2],
                domain_origin[2]);
        }
    }
}

// Check, if a particle is placed in all cells of a 3d grid, if they are mapped to the right index,
// and type of cell
TEST(CellGridTestDimensionaity, GridInit3D)
{
    std::array<double, 3> domain_origin = { -50, -50, -50 };
    std::array<double, 3> domain_size = { 15, 15, 15 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    std::vector<Particle> particles {};
    std::vector<CellIndex> indices {};

    double x = domain_origin[0] - cutoff / 2;
    size_t indexX = 0, indexY, indexZ;
    while (x < domain_origin[0] + domain_size[1] + cutoff) {
        double y = domain_origin[1] - cutoff / 2;
        indexY = 0;
        while (y < domain_origin[1] + domain_size[1] + cutoff) {
            double z = domain_origin[2] - cutoff / 2;
            indexZ = 0;
            while (z < domain_origin[2] + domain_size[2] + cutoff) {
                particles.push_back(Particle({ x, y, z }, { 0, 0, 0 }, 1.0));
                indices.push_back({ indexX, indexY, indexZ });
                z += cutoff;
                indexZ++;
            }
            y += cutoff;
            indexY++;
        }
        x += cutoff;
        indexX++;
    }

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    for (size_t i = 0; i < indices.size(); ++i) {
        CellIndex index = indices[i];
        // only one particle per cell
        EXPECT_EQ(grid.cells.at(index[0]).at(index[1]).at(index[2])->getParticles().size(), 1);
        // It is the one we expect it to be
        for (size_t ind = 0; ind < 3; ind++)
            EXPECT_EQ(
                grid.cells.at(index[0])
                    .at(index[1])
                    .at(index[2])
                    ->getParticles()
                    .front()
                    .get()
                    .getX()[ind],
                particles[i].getX()[ind]);
    }
    // check if all cells have received one particle
    EXPECT_EQ(
        particles.size(),
        (domain_size[0] / cutoff + 2) * (domain_size[1] / cutoff + 2) *
            (domain_size[2] / cutoff + 2));

    // check boundary iterator
    for (auto position : allPositions) {
        for (auto boundaryIndex : grid.boundaryCellIterator(position)) {
            // It is a boundary cell
            EXPECT_EQ(
                grid.cells.at(boundaryIndex[0])
                    .at(boundaryIndex[1])
                    .at(boundaryIndex[2])
                    ->getType(),
                CellType::Boundary);
            // Check if the particle in the cell has the expected position
            for (size_t ind = 0; ind < 3; ind++) {
                double expectedCoordinate =
                    domain_origin[ind] - cutoff / 2 + cutoff * (double)boundaryIndex[ind];
                EXPECT_EQ(
                    grid.cells.at(boundaryIndex[0])
                        .at(boundaryIndex[1])
                        .at(boundaryIndex[2])
                        ->getParticles()
                        .front()
                        .get()
                        .getX()[ind],
                    expectedCoordinate);
            }
        }
    }

    // check halo iterator
    for (auto position : allPositions) {
        for (auto haloIndex : grid.haloCellIterator(position)) {
            // It is a halo cell
            EXPECT_EQ(
                grid.cells.at(haloIndex[0]).at(haloIndex[1]).at(haloIndex[2])->getType(),
                CellType::Halo);
            // Check if the particle in the cell has the expected position
            for (size_t ind = 0; ind < 3; ind++) {
                double expectedCoordinate =
                    domain_origin[ind] - cutoff / 2 + cutoff * (double)haloIndex[ind];
                EXPECT_EQ(
                    grid.cells.at(haloIndex[0])
                        .at(haloIndex[1])
                        .at(haloIndex[2])
                        ->getParticles()
                        .front()
                        .get()
                        .getX()[ind],
                    expectedCoordinate);
            }
        }
    }
}

// Test if particle inserted at bottom is also inserted in bottom cells and will be recognized by
// the boundary iterator
TEST(CellGridOrientation, BottomParticleInBoundary)
{
    std::array<double, 3> domain_origin = { -10, -10, 0 };
    std::array<double, 3> domain_size = { 60, 40, 0 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    // Particle on bottom (paraview)
    std::vector<Particle> particles { Particle { { 0, -9.1, 0 }, { 0, -30, 0 }, 1 } };

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    size_t particleCount = 0;
    for (auto index : grid.boundaryCellIterator(BOTTOM)) {
        particleCount += grid.cells[index[0]][index[1]][index[2]]->getParticles().size();
    }

    EXPECT_EQ(particleCount, 1);
}

// Test if particle inserted at top is also inserted in bottom cells and will be recognized by
// the boundary iterator
TEST(CellGridOrientation, TopParticleInBoundary)
{
    std::array<double, 3> domain_origin = { -10, -10, 0 };
    std::array<double, 3> domain_size = { 60, 40, 0 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    // Particle on bottom (paraview)
    std::vector<Particle> particles { Particle { { 0, 29.1, 0 }, { 0, -30, 0 }, 1 } };

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    size_t particleCount = 0;
    for (auto index : grid.boundaryCellIterator(TOP)) {
        particleCount += grid.cells[index[0]][index[1]][index[2]]->getParticles().size();
    }

    EXPECT_EQ(particleCount, 1);
}

// Test if particle inserted left is also inserted in bottom cells and will be recognized by the
// boundary iterator
TEST(CellGridOrientation, LeftParticleInBoundary)
{
    std::array<double, 3> domain_origin = { -10, -10, 0 };
    std::array<double, 3> domain_size = { 60, 40, 0 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    // Particle left side (paraview)
    std::vector<Particle> particles { Particle { { -9.1, 0, 0 }, { 0, -30, 0 }, 1 } };

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    size_t particleCount = 0;
    for (auto index : grid.boundaryCellIterator(LEFT)) {
        particleCount += grid.cells[index[0]][index[1]][index[2]]->getParticles().size();
    }

    EXPECT_EQ(particleCount, 1);
}

// Test if particle inserted right is also inserted in bottom cells and will be recognized by the
// boundary iterator
TEST(CellGridOrientation, RightParticleInBoundary)
{
    std::array<double, 3> domain_origin = { -10, -10, 0 };
    std::array<double, 3> domain_size = { 60, 40, 0 };
    double cutoff = 5;
    CellGrid grid { domain_origin, domain_size, cutoff };

    // Particle left side (paraview)
    std::vector<Particle> particles { Particle { { 49.1, 0, 0 }, { 0, -30, 0 }, 1 } };

    ParticleContainer container(particles);
    grid.addParticlesFromContainer(container);

    size_t particleCount = 0;
    for (auto index : grid.boundaryCellIterator(RIGHT)) {
        particleCount += grid.cells[index[0]][index[1]][index[2]]->getParticles().size();
    }

    EXPECT_EQ(particleCount, 1);
}
