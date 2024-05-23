
#include "CellGrid.h"
#include <cmath>
#include <stdexcept>

CellGrid::CellGrid(const std::array<double, 3>& domainSize, double cutoffRadius)
    : domainSize(domainSize), cutoffRadius(cutoffRadius), gridDimensions({0, 0, 0}) {
    initializeGrid();
}

CellGrid::~CellGrid() = default;

void CellGrid::addParticle(std::unique_ptr<Particle> particle) {
    auto pos = particle->getX();
    std::array<size_t, 3> indices{0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        if (pos[i] < 0) {
            indices[i] = 0;
        } else if (pos[i] >= domainSize[i]) {
            indices[i] = gridDimensions[i] - 1;
        } else {
            indices[i] = static_cast<size_t>(pos[i] / cutoffRadius) + 1;
        }
    }

    cells[indices[0]][indices[1]][indices[2]].addParticle(std::move(particle));
}

void CellGrid::addParticlesFromContainer(const ParticleContainer& particleContainer) {
    for (const auto& particle : particleContainer.particles) {
        auto uniqueParticle = std::make_unique<Particle>(particle);
        addParticle(std::move(uniqueParticle));
    }
}

const std::vector<std::vector<std::vector<Cell>>>& CellGrid::getCells() const {
    return cells;
}

void CellGrid::initializeGrid() {
    for (int i = 0; i < 3; ++i) {
        gridDimensions[i] = static_cast<size_t>(std::ceil(domainSize[i] / cutoffRadius)) + 2; // Extra cells for halo
    }

    cells.resize(gridDimensions[0]);
    for (size_t x = 0; x < gridDimensions[0]; ++x) {
        cells[x].resize(gridDimensions[1]);
        for (size_t y = 0; y < gridDimensions[1]; ++y) {
            cells[x][y].resize(gridDimensions[2]);
            for (size_t z = 0; z < gridDimensions[2]; ++z) {
                cells[x][y][z] = Cell(determineCellType({x, y, z}));
            }
        }
    }
}

CellType CellGrid::determineCellType(const std::array<size_t, 3>& indices) const {
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

// BoundaryIterator Implementation

CellGrid::BoundaryIterator::BoundaryIterator(std::vector<std::vector<std::vector<Cell>>>& cells, bool end)
    : cells(cells), x(0), y(0), z(0) {
    if (!end) {
        // Initialize to the first boundary particle
        advance();
    } else {
        // Initialize to the end state
        x = cells.size();
    }
}

std::unique_ptr<Particle>& CellGrid::BoundaryIterator::operator*() const {
    return *particleIt;
}

CellGrid::BoundaryIterator& CellGrid::BoundaryIterator::operator++() {
    ++particleIt;
    advance();
    return *this;
}

bool CellGrid::BoundaryIterator::operator!=(const BoundaryIterator& other) const {
    return x != other.x || y != other.y || z != other.z || particleIt != other.particleIt;
}

void CellGrid::BoundaryIterator::advance() {
    while (x < cells.size()) {
        while (y < cells[x].size()) {
            while (z < cells[x][y].size()) {
                if (cells[x][y][z].getType() == CellType::Boundary) {
                    if (particleIt == cells[x][y][z].getParticles().end()) {
                        ++z;
                        if (z < cells[x][y].size()) {
                            particleIt = cells[x][y][z].getParticles().begin();
                        }
                    } else {
                        return;
                    }
                } else {
                    ++z;
                }
            }
            z = 0;
            ++y;
        }
        y = 0;
        ++x;
    }
}

// HaloIterator Implementation

CellGrid::HaloIterator::HaloIterator(std::vector<std::vector<std::vector<Cell>>>& cells, bool end)
    : cells(cells), x(0), y(0), z(0) {
    if (!end) {
        // Initialize to the first halo particle
        advance();
    } else {
        // Initialize to the end state
        x = cells.size();
    }
}

std::unique_ptr<Particle>& CellGrid::HaloIterator::operator*() const {
    return *particleIt;
}

CellGrid::HaloIterator& CellGrid::HaloIterator::operator++() {
    ++particleIt;
    advance();
    return *this;
}

bool CellGrid::HaloIterator::operator!=(const HaloIterator& other) const {
    return x != other.x || y != other.y || z != other.z || particleIt != other.particleIt;
}

void CellGrid::HaloIterator::advance() {
    while (x < cells.size()) {
        while (y < cells[x].size()) {
            while (z < cells[x][y].size()) {
                if (cells[x][y][z].getType() == CellType::Halo) {
                    if (particleIt == cells[x][y][z].getParticles().end()) {
                        ++z;
                        if (z < cells[x][y].size()) {
                            particleIt = cells[x][y][z].getParticles().begin();
                        }
                    } else {
                        return;
                    }
                } else {
                    ++z;
                }
            }
            z = 0;
            ++y;
        }
        y = 0;
        ++x;
    }
}

// Methods to get boundary and halo particle iterators
CellGrid::BoundaryIterator CellGrid::beginBoundaryParticles() {
    return BoundaryIterator(cells);
}

CellGrid::BoundaryIterator CellGrid::endBoundaryParticles() {
    return BoundaryIterator(cells, true);
}

CellGrid::HaloIterator CellGrid::beginHaloParticles() {
    return HaloIterator(cells);
}

CellGrid::HaloIterator CellGrid::endHaloParticles() {
    return HaloIterator(cells, true);
}