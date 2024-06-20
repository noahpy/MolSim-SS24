
#include "models/linked_cell/CellGrid.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/boundaryConditions/PeriodicBoundary.h"
#include <gtest/gtest.h>

// Check if the soft reflective boundary will correctly precompute the translations needed by
// checking the calculated ones against hand calculated shifts (in a 2D case)
TEST(PeriodicBoundary, CorrectInitAndTranslation2D)
{
    BoundaryConfig config(
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC);
    std::array<double, 3> domainOrigin { 0, 0, 0 };
    std::array<double, 3> domainSize { 10, 10, 0 };
    double cutoffRadius = 5;
    CellGrid grid(domainOrigin, domainSize, cutoffRadius);
    PeriodicBoundary boundary(RIGHT, config, grid);

    std::vector<Position> upRight { Position::RIGHT, Position::TOP };
    CellIndex upRightIndex = { 2, 2, 0 };
    // The upper right cell should have 3 positions to insert a halo particle at
    EXPECT_EQ(boundary.getTranslationMap().at(upRight).size(), 3);

    // Indices we expect to be found
    std::vector<CellIndex> expectedPositions { { 0, 2, 0 }, { 2, 0, 0 }, { 0, 0, 0 } };
    std::vector<bool> foundPositions(3, false);

    // Check if the calculated shifts are correct
    auto map = boundary.getTranslationMap().at(upRight);
    for (auto shifts : map) {
        CellIndex calcIndex = { (size_t)((int)upRightIndex[0] + shifts.second[0]),
                                (size_t)((int)upRightIndex[1] + shifts.second[1]),
                                (size_t)((int)upRightIndex[2] + shifts.second[2]) };
        for (size_t i = 0; i < expectedPositions.size(); i++)
            for (size_t j = 0; j < 3; j++) {
                if (calcIndex[j] != expectedPositions[i][j])
                    break;
                if (j == 2)
                    foundPositions[i] = true;
            }
    }
    for (const auto& foundPosition : foundPositions)
        EXPECT_TRUE(foundPosition);
}

// Check if the soft reflective boundary will correctly precompute the translations needed by
// checking the calculated ones against hand calculated shifts (in a 3D case)
TEST(PeriodicBoundary, CorrectInitAndTranslation3D)
{
    BoundaryConfig config(
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC,
        BoundaryType::PERIODIC);
    std::array<double, 3> domainOrigin { 0, 0, 0 };
    std::array<double, 3> domainSize { 10, 10, 10 };
    double cutoffRadius = 5;
    CellGrid grid(domainOrigin, domainSize, cutoffRadius);
    PeriodicBoundary boundary(RIGHT, config, grid);

    std::vector<Position> upRight { Position::RIGHT, Position::TOP, Position::FRONT };
    CellIndex upRightFrontIndex = { 2, 2, 2 };
    // The upper right cell should have 3 positions to insert a halo particle at
    EXPECT_EQ(boundary.getTranslationMap().at(upRight).size(), 3 + 3 + 1);

    // Indices we expect to be found
    std::vector<CellIndex> expectedPositions { { 0, 2, 2 }, { 2, 0, 2 }, { 2, 2, 0 }, { 0, 0, 2 },
                                               { 2, 0, 0 }, { 0, 2, 0 }, { 0, 0, 0 } };
    std::vector<bool> foundPositions(expectedPositions.size(), false);

    // Check if the calculated shifts are correct
    auto map = boundary.getTranslationMap().at(upRight);
    for (auto shifts : map) {
        CellIndex calcIndex = { (size_t)((int)upRightFrontIndex[0] + shifts.second[0]),
                                (size_t)((int)upRightFrontIndex[1] + shifts.second[1]),
                                (size_t)((int)upRightFrontIndex[2] + shifts.second[2]) };
        for (size_t i = 0; i < expectedPositions.size(); i++)
            for (size_t j = 0; j < 3; j++) {
                if (calcIndex[j] != expectedPositions[i][j])
                    break;
                if (j == 2)
                    foundPositions[i] = true;
            }
    }
    for (const auto& foundPosition : foundPositions)
        EXPECT_TRUE(foundPosition);

    // The entries should not have been deleted
    EXPECT_EQ(boundary.getTranslationMap().at(upRight).size(), 3 + 3 + 1);
}
