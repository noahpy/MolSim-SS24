
#include "CellGrid.h"
#include "models/linked_cell/cell/Cell.h"
#include "utils/ArrayUtils.h"
#include "utils/Position.h"
#include <cmath>
#include <spdlog/spdlog.h>

CellGrid::CellGrid(
    const std::array<double, 3> domainOrigin,
    const std::array<double, 3> domainSize,
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

void CellGrid::determineNeighbours(CellIndex cell)
{
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                if (i == 0 && j == 0 && k == 0) {
                    continue;
                }
                // check if cell is in bounds
                if (cell[0] + i < 0 || cell[0] + i >= gridDimensions[0] || cell[1] + j < 0 ||
                    cell[1] + j >= gridDimensions[1] || cell[2] + k < 0 ||
                    cell[2] + k >= gridDimensions[2]) {
                    continue;
                }
                CellIndex neighbourIndex = { cell[0] + i, cell[1] + j, cell[2] + k };

                auto positions = relCoordinateToPos({ static_cast<unsigned long>(i),
                                                      static_cast<unsigned long>(j),
                                                      static_cast<unsigned long>(k) });

                switch (determineCellType(neighbourIndex)) {
                case CellType::Boundary:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->boundaryNeighbours.emplace_back(
                        std::make_pair(neighbourIndex, positions));
                    break;
                case CellType::Halo:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->haloNeighbours.emplace_back(
                        std::make_pair(neighbourIndex, positions));
                    break;
                case CellType::Inner:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->innerNeighbours.emplace_back(
                        std::make_pair(neighbourIndex, positions));
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void CellGrid::initializeGrid()
{
    for (int i = 0; i < 3; ++i) {
        // if dimension is zero
        if (domainSize[i] == 0) {
            spdlog::error("Domain size cannot be zero!");
            exit(EXIT_FAILURE);
        }
        // if cutoff radius is bigger than domain size
        if (domainSize[i] < cutoffRadius) {
            cellSize[i] = domainSize[i];
            gridDimensions[i] = 3;
            continue;
        }
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
                std::unique_ptr<Cell> cellPointer = std::make_unique<Cell>(Cell(type, { x, y, z }));
                // save pointer
                cells.at(x).at(y).at(z) = std::move(cellPointer);
                // determine neighbours
                determineNeighbours({ x, y, z });
                if(type == CellType::Boundary) {
                    boundaryCells.push_back({ x, y, z });
                }
                if(type == CellType::Halo) {
                    haloCells.push_back({ x, y, z });
                }
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

CellIndex CellGrid::getIndexFromPos(const std::array<double, 3>& ppos) const
{
    auto pos = ppos - domainOrigin;
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
    return indices;
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
                    CellIndex indices = getIndexFromPos((*it).get().getX());
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
    CellIndex indices = getIndexFromPos(particle.getX());

    cells.at(indices[0]).at(indices[1]).at(indices[2])->addParticle(particle);
    spdlog::debug(
        "Particle {} added to cell ({}, {}, {})",
        particle.toString(),
        indices[0],
        indices[1],
        indices[2]);
}

void CellGrid::addParticlesFromContainer(ParticleContainer& particleContainer)
{
    for (auto& particle : particleContainer.particles) {
        addParticle(particle);
    }
}

std::list<CellIndex> CellGrid::getNeighbourCells(const CellIndex& cellIndex) const
{
    std::list<CellIndex> cellList;

    // If not all neighbours have been paired up, return an empty list
    // If the cell is a halo, return an empty list
    if (cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->getCounter() > 0 ||
        cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->getType() == CellType::Halo) {
        return cellList;
    }

    // check if this cell was not visited
    if (!cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->visited) {
        // Mark this cell as visited
        cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->visited = true;
        // Set forces
        auto particleRefs =
            cells.at(cellIndex[0]).at(cellIndex[1]).at(cellIndex[2])->getParticles();
        for (auto& p : particleRefs) {
            p.get().setOldF(p.get().getF());
            p.get().setF({ 0.0, 0.0, 0.0 });
        }
    }

    // Iterate over all neighbors including the cell itself
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                size_t nx = cellIndex[0] + dx;
                size_t ny = cellIndex[1] + dy;
                size_t nz = cellIndex[2] + dz;

                // Skip if it is the original cell
                if (dx == 0 && dy == 0 && dz == 0) {
                    continue;
                }

                if (nx < gridDimensions[0] && ny < gridDimensions[1] && nz < gridDimensions[2]) {
                    if (cells.at(nx).at(ny).at(nz)->getCounter() > 0) {
                        // Decrease counter
                        cells.at(nx).at(ny).at(nz)->decrementCounter();
                        // if counter reached zero, reset visited
                        if (cells.at(nx).at(ny).at(nz)->getCounter() == 0) {
                            cells.at(nx).at(ny).at(nz)->visited = false;
                        }
                        // Skip this neighbour
                        continue;
                    }

                    // Insert all particles into particleList
                    cellList.emplace_back(CellIndex { nx, ny, nz });

                    // check if not visited yet
                    if (!cells.at(nx).at(ny).at(nz)->visited) {
                        // Mark as visited
                        cells.at(nx).at(ny).at(nz)->visited = true;
                        // Set OldF to F and zero F
                        auto particleRefs = cells.at(nx).at(ny).at(nz)->getParticles();
                        for (auto& p : particleRefs) {
                            p.get().setOldF(p.get().getF());
                            p.get().setF({ 0.0, 0.0, 0.0 });
                        }
                    }

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

    return cellList;
}
