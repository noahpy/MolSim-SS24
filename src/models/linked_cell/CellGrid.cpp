
#include "CellGrid.h"
#include "models/linked_cell/BoundaryCell.h"
#include "models/linked_cell/Cell.h"
#include "models/linked_cell/HaloCell.h"
#include "models/linked_cell/InnerCell.h"
#include <cmath>

CellGrid::CellGrid(const std::array<double, 3>& domainSize, double cutoffRadius)
    : domainSize(domainSize)
    , cutoffRadius(cutoffRadius)
    , gridDimensions({ 0, 0, 0 })
{
    initializeGrid();
}

CellGrid::~CellGrid() = default;

void CellGrid::initializeGrid()
{
    for (int i = 0; i < 3; ++i) {
        // Extra cells for halo
        gridDimensions[i] = static_cast<size_t>(std::ceil(domainSize[i] / cutoffRadius)) + 2;
    }

    cells.resize(gridDimensions[0]);
    for (size_t x = 0; x < gridDimensions[0]; ++x) {
        cells[x].resize(gridDimensions[1]);
        for (size_t y = 0; y < gridDimensions[1]; ++y) {
            cells[x][y].resize(gridDimensions[2]);
            for (size_t z = 0; z < gridDimensions[2]; ++z) {
                CellType type = determineCellType({ x, y, z });
                // Initialize vectors for boundary and halo cells
                std::unique_ptr<Cell> cellPointer;
                if (type == CellType::Boundary) {
                    cellPointer = std::make_unique<BoundaryCell>(BoundaryCell({ x, y, z }));
                    boundaryCells.push_back({ x, y, z });
                } else if (type == CellType::Halo) {
                    cellPointer = std::make_unique<HaloCell>(HaloCell({ x, y, z }));
                    haloCells.push_back({ x, y, z });
                } else {
                    cellPointer = std::make_unique<InnerCell>(InnerCell());
                }
                cells[x][y][z] = std::move(cellPointer);
            }
        }
    }
}

CellType CellGrid::determineCellType(const std::array<size_t, 3>& indices) const
{
    for (int i = 0; i < 3; ++i) {
        if (indices[i] == 0 || indices[i] == gridDimensions[i] - 1) {
            return CellType::Halo;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (indices[i] == 1 || indices[i] == gridDimensions[i] - 2) {
            return CellType::Boundary;
        }
    }
    return CellType::Inner;
}

// Methods to get boundary and halo particle iterators
CellGrid::BoundaryIterator CellGrid::beginBoundaryParticles()
{
    return BoundaryIterator(boundaryCells, false);
}

CellGrid::BoundaryIterator CellGrid::endBoundaryParticles()
{
    return BoundaryIterator(boundaryCells, true);
}

CellGrid::HaloIterator CellGrid::beginHaloParticles()
{
    return HaloIterator(haloCells, false);
}

CellGrid::HaloIterator CellGrid::endHaloParticles()
{
    return HaloIterator(haloCells, true);
}

void CellGrid::addParticle(Particle& particle)
{
    auto pos = particle.getX();
    std::array<size_t, 3> indices { 0, 0, 0 };

    for (int i = 0; i < 3; ++i) {
        if (pos[i] < 0) {
            indices[i] = 0;
        } else if (pos[i] >= domainSize[i]) {
            indices[i] = gridDimensions[i] - 1;
        } else {
            indices[i] = static_cast<size_t>(pos[i] / cutoffRadius) + 1;
        }
    }

    cells[indices[0]][indices[1]][indices[2]]->addParticle(particle);
}

void CellGrid::addParticlesFromContainer(ParticleContainer& particleContainer)
{
    for (auto& particle : particleContainer.particles) {
        addParticle(particle);
    }
}

std::list<std::reference_wrapper<Particle>> CellGrid::getNeighboringParticles(
    const std::array<size_t, 3>& cellIndex)
{
    std::list<std::reference_wrapper<Particle>> particleList;

    // Check if the cell at the given index is a halo cell
    if (cells[cellIndex[0]][cellIndex[1]][cellIndex[2]]->getType() == CellType::Halo) {
        return particleList; // Return empty list if the cell is a halo cell
    }

    // Iterate over all neighbors including the cell itself
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                size_t nx = cellIndex[0] + dx;
                size_t ny = cellIndex[1] + dy;
                size_t nz = cellIndex[2] + dz;

                if (nx < gridDimensions[0] && ny < gridDimensions[1] && nz < gridDimensions[2]) {
                    // Check if the neighboring cell is a halo cell or if its neighborCounter > 0
                    if (cells[nx][ny][nz]->getType() != CellType::Halo) {
                        if (cells[nx][ny][nz]->getCounter() > 0) {
                            cells[nx][ny][nz]->setCounter(
                                cells[nx][ny][nz]->getCounter() -
                                1); // Decrease the neighborCounter
                            continue; // Skip this neighbor to ensure the unique pair iteration
                        }

                        // Add particle pointer from neighbor to the main list
                        const auto& neighborParticles = cells[nx][ny][nz]->getParticles();
                        for (const auto& particle : neighborParticles) {
                            particleList.push_back(particle);
                        }
                        // Increment neighborCounter if particles from a neighbor are added
                        cells[cellIndex[0]][cellIndex[1]][cellIndex[2]]->setCounter(
                            cells[cellIndex[0]][cellIndex[1]][cellIndex[2]]->getCounter() + 1);
                    }
                }
            }
        }
    }

    return particleList;
}
