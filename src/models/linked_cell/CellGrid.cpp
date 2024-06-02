
#include "CellGrid.h"
#include "models/linked_cell/cell/BoundaryCell.h"
#include "models/linked_cell/cell/Cell.h"
#include "models/linked_cell/cell/HaloCell.h"
#include "models/linked_cell/cell/InnerCell.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include <iostream>

CellGrid::CellGrid(
    const std::array<double, 3> domainOrigin,
    const std::array<double, 3>& domainSize,
    double cutoffRadius)
    : domainOrigin(domainOrigin)
    , domainSize(domainSize)
    , cutoffRadius(cutoffRadius)
    , gridDimensions({ 0, 0, 0 })
    , cellSize({ 0.0, 0.0, 0.0 })
{
    initializeGrid();
}

CellGrid::~CellGrid() = default;

void CellGrid::initializeGrid()
{
    for (int i = 0; i < 3; ++i) {
        gridDimensions[i] = static_cast<size_t>(std::floor(domainSize[i] / cutoffRadius));
        cellSize[i] = domainSize[i] / static_cast<double>(gridDimensions[i]);
        gridDimensions[i] += 2;
    }

    cells.resize(gridDimensions[0]);
    for (size_t x = 0; x < gridDimensions[0]; ++x) {
        cells.at(x).resize(gridDimensions[1]);
        for (size_t y = 0; y < gridDimensions[1]; ++y) {
            cells.at(x).at(y).resize(gridDimensions[2]);
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
                cells.at(x).at(y).at(z) = std::move(cellPointer);
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

void CellGrid::updateCells()
{
    std::list<std::pair<CellIndex, std::reference_wrapper<Particle>>> addList;
    for (size_t x = 0; x < gridDimensions[0]; ++x) {
        for (size_t y = 0; y < gridDimensions[1]; ++y) {
            for (size_t z = 0; z < gridDimensions[2]; ++z) {
                ParticleRefList& particles = cells.at(x).at(y).at(z)->getParticles();
                ParticleRefList::iterator it = particles.begin();
                while (it != particles.end()) {
                    // Calculate new index
                    auto pos = (*it).get().getX() - domainOrigin;
                    CellIndex indices { 0, 0, 0 };
                    for (int i = 0; i < 3; ++i) {
                        if (pos[i] < 0) {
                            indices[i] = 0;
                        } else if (pos[i] >= domainSize[i]) {
                            indices[i] = gridDimensions[i] - 1;
                        } else {
                            indices[i] = static_cast<size_t>(pos[i] / cellSize[i]) + 1;
                        }
                    }
                    // Add the particle to different cell if particle moved
                    if (indices.at(0) != x || indices.at(1) != y || indices.at(2) != z) {
                        // Save addition for later
                        addList.emplace_back(indices, *it);
                        // Remove particle from old cell
                        particles.erase(it++);
                    } else {
                        ++it;
                    }
                }
            }
        }
    }

    // Add particles to their new cells
    for (auto& p : addList) {
        cells.at(p.first.at(0)).at(p.first.at(1)).at(p.first.at(2))->addParticle(p.second);
    }
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
    auto pos = particle.getX() - domainOrigin;
    std::array<size_t, 3> indices { 0, 0, 0 };

    for (int i = 0; i < 3; ++i) {
        if (pos[i] < 0) {
            indices[i] = 0;
        } else if (pos[i] >= domainSize[i]) {
            indices[i] = gridDimensions[i] - 1;
        } else {
            indices[i] = static_cast<size_t>(pos[i] / cellSize[i]) + 1;
        }
    }

    cells.at(indices[0]).at(indices[1]).at(indices[2])->addParticle(particle);
}

void CellGrid::addParticlesFromContainer(ParticleContainer& particleContainer)
{
    for (auto& particle : particleContainer.particles) {
        addParticle(particle);
    }
}

ParticleRefList CellGrid::getNeighboringParticles(const CellIndex& cellIndex)
{
    ParticleRefList particleList;

    // If not all neighbours have been paired up, return an empty list
    if (cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->getCounter() > 0) {
        return particleList;
    }

    // Iterate over all neighbors including the cell itself
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                size_t nx = cellIndex[0] + dx;
                size_t ny = cellIndex[1] + dy;
                size_t nz = cellIndex[2] + dz;

                // Skip if it is the original cell
                if (dx + dy + dz == 0) {
                    continue;
                }

                if (nx < gridDimensions[0] && ny < gridDimensions[1] && nz < gridDimensions[2]) {
                    if (cells.at(nx).at(ny).at(nz)->getCounter() > 0) {
                        // Decrease counter
                        cells.at(nx).at(ny).at(nz)->decrementCounter();
                        // Skip this neighbour
                        continue;
                    }

                    // Add particle pointer from neighbor to the main list
                    ParticleRefList& neighborParticles = cells.at(nx).at(ny).at(nz)->getParticles();
                    // Insert all particles into particleList
                    particleList.insert(
                        particleList.end(), neighborParticles.begin(), neighborParticles.end());

                    if (cells.at(nx).at(ny).at(nz)->getType() != CellType::Halo) {
                        // Increment neighborCounter if not a halo cell
                        cells.at(cellIndex[0])
                            .at(cellIndex[1])
                            .at(cellIndex[2])
                            ->incrementCounter();
                    }
                }
            }
        }
    }

    return particleList;
}
