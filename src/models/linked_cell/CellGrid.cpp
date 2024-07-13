
#include "CellGrid.h"
#include "models/ParticleContainer.h"
#include "models/linked_cell/cell/Cell.h"
#include "utils/ArrayUtils.h"
#include "utils/Position.h"
#include <cmath>
#include <cwchar>
#include <spdlog/spdlog.h>

CellGrid::CellGrid(
    const std::array<double, 3> domainOrigin,
    const std::array<double, 3> domainSize,
    double cutoffRadius)
    : domainOrigin(domainOrigin)
    , domainSize(domainSize)
    , cutoffRadius(cutoffRadius)
    , cutoffRadiusSquared(cutoffRadius * cutoffRadius)
    , gridDimensions({ 0, 0, 0 })
    , cellSize({ 0.0, 0.0, 0.0 })
    , domainEnd(domainOrigin + domainSize)
    , gridDimensionality((domainSize[2] == 0) ? 2 : 3)
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

                auto positions = relCoordinateToPos({ i, j, k });
                switch (determineCellType(neighbourIndex)) {
                case CellType::Boundary:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->boundaryNeighbours.emplace_back(
                        neighbourIndex, positions);
                    break;
                case CellType::Halo:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->haloNeighbours.emplace_back(
                        neighbourIndex, positions);
                    break;
                case CellType::Inner:
                    cells.at(cell[0]).at(cell[1]).at(cell[2])->innerNeighbours.emplace_back(
                        neighbourIndex, positions);
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
    if (gridDimensionality == 2) {
        spdlog::info("Domain is set to 0 in z-Axis. Grid will be constructed in 2D.");
        gridDimensions[2] = 1; // a single cell
        cellSize[2] = 0; // no depth
    }

    for (int i = 0; i < gridDimensionality; ++i) {
        // if dimension is zero
        if (domainSize[i] == 0) {
            spdlog::error("Domain size of x-Axis or y-Axis cannot be zero!");
            exit(EXIT_FAILURE);
        }
        // if cutoff radius is bigger than domain size
        if (domainSize[i] < cutoffRadius) {
            cellSize[i] = domainSize[i];
            gridDimensions[i] = 3;
            continue;
        }
        gridDimensions[i] = static_cast<size_t>(std::floor(std::abs(domainSize[i] / cutoffRadius)));
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
                if (type == CellType::Boundary) {
                    boundaryCells.push_back({ x, y, z });
                }
                if (type == CellType::Halo) {
                    haloCells.push_back({ x, y, z });
                }
            }
        }
    }
}

CellType CellGrid::determineCellType(const std::array<size_t, 3>& indices) const
{
    for (int i = 0; i < gridDimensionality; ++i) {
        if (indices[i] == 0 || indices[i] == gridDimensions[i] - 1) {
            return CellType::Halo;
        }
    }
    for (int i = 0; i < gridDimensionality; ++i) {
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

    for (int i = 0; i < gridDimensionality; ++i) {
        if (pos[i] < 0) {
            indices[i] = 0;
        } else if (pos[i] >= domainSize[i]) {
            indices[i] = gridDimensions[i] - 1;
        } else {
            indices[i] = static_cast<size_t>(pos[i] / cellSize[i]) + 1;
        }
    }
    // if gridDimensionality == 2, indices[2] will remain 0 from initialization

    return indices;
}

void CellGrid::updateCells()
{
    spdlog::debug("Updating cells...");
    std::list<std::pair<CellIndex, std::reference_wrapper<Particle>>> addList;
    for (size_t x = 0; x < gridDimensions[0]; ++x) {
        for (size_t y = 0; y < gridDimensions[1]; ++y) {
            for (size_t z = 0; z < gridDimensions[2]; ++z) {
                ParticleRefList& particles = cells.at(x).at(y).at(z)->getParticles();
                auto it = particles.begin();
                while (it != particles.end()) {
                    // Calculate new index
                    CellIndex indices = getIndexFromPos((*it).get().getX());
                    // Add the particle to different cell if particle moved
                    if (indices.at(0) != x || indices.at(1) != y || indices.at(2) != z) {
                        // Save addition for later
                        addList.emplace_back(indices, *it);
                        // Remove particle from old cell
                        it = particles.erase(it);
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
CellGrid::BoundaryIterator CellGrid::boundaryCellIterator(Position position) const
{
    return { position, gridDimensions, gridDimensionality == 2 };
}

CellGrid::HaloIterator CellGrid::haloCellIterator(Position position) const
{
    return { position, gridDimensions, gridDimensionality == 2 };
}

void CellGrid::addParticle(Particle& particle)
{
    CellIndex indices = getIndexFromPos(particle.getX());

    cells.at(indices[0]).at(indices[1]).at(indices[2])->addParticle(particle);
}

void CellGrid::addParticlesFromContainer(ParticleContainer& particleContainer)
{
    for (auto& particle : particleContainer.particles) {
        addParticle(particle);
    }
}

void CellGrid::preCalcSetup(ParticleContainer& container) const
{
    spdlog::debug("Pre-calc setup...");
#pragma omp parallel for
    for (auto& particle : container) {
        particle.resetF();
    }
}

void CellGrid::postCalcSetup() const
{
    spdlog::debug("Post-calc setup...");
#pragma omp parallel for
    for (auto& cell : cells) {
        for (auto& row : cell) {
            for (auto& col : row) {
                col->unvisit();
            }
        }
    }
}

std::list<CellIndex> CellGrid::getNeighbourCells(const CellIndex& index) const THREAD_SAFE
{
    std::list<CellIndex> cellList;

    // If the cell is a halo, return an empty list
    if (cells.at(index[0]).at(index[1]).at(index[2])->getType() == CellType::Halo) {
        return cellList;
    }

    // If the cell has already been visited, return an empty list
    // and if not, flip the visit flag
    if (cells.at(index[0]).at(index[1]).at(index[2])->visit()) {
        return cellList;
    }

    // Iterate over all neighbors
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                // Skip if it is the original cell
                if (dx == 0 && dy == 0 && dz == 0) {
                    continue;
                }

                size_t nx = index[0] + dx;
                size_t ny = index[1] + dy;
                size_t nz = index[2] + dz;

                // Continue if the neighbor is in bounds
                if (nx < gridDimensions[0] && ny < gridDimensions[1] && nz < gridDimensions[2]) {
                    // if the neighbour is already visited, continue
                    if (cells.at(nx).at(ny).at(nz)->getVisited()) {
                        continue;
                    }

                    // Insert all particles into particleList
                    cellList.emplace_back(CellIndex { nx, ny, nz });
                }
            }
        }
    }
    return cellList;
}

void CellGrid::setCutoffRadius(double pCutoffRadius)
{
    cutoffRadius = pCutoffRadius;
}

void CellGrid::setDomainSize(const std::array<double, 3>& pDomainSize)
{
    domainSize = pDomainSize;
}

void CellGrid::setDomainOrigin(const std::array<double, 3>& pDomainOrigin)
{
    domainOrigin = pDomainOrigin;
}
