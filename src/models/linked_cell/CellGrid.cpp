
#include "CellGrid.h"
#include <cmath>

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
                CellType type = determineCellType({x, y, z});
                cells[x][y][z] = Cell(type);
                if (type == CellType::Boundary) boundaryCells.push_back(cells[x][y][z]);
                else if (type == CellType::Halo) haloCells.push_back(cells[x][y][z]);
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

CellGrid::BoundaryIterator::BoundaryIterator(std::vector<Cell>& boundaryCells, bool end)
    : boundaryCells(boundaryCells), cellIndex(0) {
    if (!end) {
        // Initialize to the first boundary particle
        advance();
    } else {
        // Initialize to the end state
        cellIndex = boundaryCells.size();
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
    return cellIndex != other.cellIndex || particleIt != other.particleIt;
}

void CellGrid::BoundaryIterator::advance() {
    while (cellIndex < boundaryCells.size()) {
        if (particleIt == boundaryCells[cellIndex].getParticles().end()) {
            ++cellIndex;
            if (cellIndex < boundaryCells.size()) {
                particleIt = boundaryCells[cellIndex].getParticles().begin();
            }
        } else {
            return;
        }
    }
}

// HaloIterator Implementation

CellGrid::HaloIterator::HaloIterator(std::vector<Cell>& haloCells, bool end)
    : haloCells(haloCells), cellIndex(0) {
    if (!end) {
        // Initialize to the first halo particle
        advance();
    } else {
        // Initialize to the end state
        cellIndex = haloCells.size();
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
    return cellIndex != other.cellIndex || particleIt != other.particleIt;
}

void CellGrid::HaloIterator::advance() {
    while (cellIndex < haloCells.size()) {
        if (particleIt == haloCells[cellIndex].getParticles().end()) {
            ++cellIndex;
            if (cellIndex < haloCells.size()) {
                particleIt = haloCells[cellIndex].getParticles().begin();
            }
        } else {
            return;
        }
    }
}

// Methods to get boundary and halo particle iterators
CellGrid::BoundaryIterator CellGrid::beginBoundaryParticles() {
    return BoundaryIterator(boundaryCells, false);
}

CellGrid::BoundaryIterator CellGrid::endBoundaryParticles() {
    return BoundaryIterator(boundaryCells, true);
}

CellGrid::HaloIterator CellGrid::beginHaloParticles() {
    return HaloIterator(haloCells, false);
}

CellGrid::HaloIterator CellGrid::endHaloParticles() {
    return HaloIterator(haloCells, true);
}

// PairIterator Implementation

CellGrid::PairIterator::PairIterator(std::list<std::unique_ptr<Particle>>& particles, bool end)
    : particles(particles), firstIt(particles.begin()), secondIt(particles.begin()) {
    if (!end) {
        if (secondIt != particles.end()) {
            ++secondIt;
        }
    } else {
        firstIt = particles.end();
        secondIt = particles.end();
    }
}

std::pair<Particle*, Particle*> CellGrid::PairIterator::operator*() const {
    return {firstIt->get(), secondIt->get()};
}

CellGrid::PairIterator& CellGrid::PairIterator::operator++() {
    advance();
    return *this;
}

bool CellGrid::PairIterator::operator!=(const PairIterator& other) const {
    return firstIt != other.firstIt || secondIt != other.secondIt;
}

void CellGrid::PairIterator::advance() {
    if (firstIt == particles.end()) {
        return;
    }

    if (secondIt == particles.end()) {
        ++firstIt;
        if (firstIt != particles.end()) {
            secondIt = std::next(firstIt);
        }
    } else ++secondIt;
}

CellGrid::PairIterator CellGrid::PairIterator::beginPairs(std::list<std::unique_ptr<Particle>>& particles) {
    return PairIterator(particles, false);
}

CellGrid::PairIterator CellGrid::PairIterator::endPairs(std::list<std::unique_ptr<Particle>>& particles) {
    return PairIterator(particles, true);
}
