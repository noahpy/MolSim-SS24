
#pragma once

#include <vector>
#include <array>
#include "Cell.h"
#include "CellType.h"
#include "models/ParticleContainer.h"

// Class representing the grid of cells
class CellGrid {
public:
    // Constructor
    CellGrid(const std::array<double, 3>& domainSize, double cutoffRadius);

    // Destructor
    ~CellGrid();

    // Method to add a particle to the grid
    void addParticle(std::unique_ptr<Particle> particle);

    // Method to add particles from ParticleContainer
    void addParticlesFromContainer(const ParticleContainer& particleContainer);

    // Method to get all cells
    [[nodiscard]] const std::vector<std::vector<std::vector<Cell>>>& getCells() const;

    // Iterator for boundary particles
    class BoundaryIterator {
    public:
        explicit BoundaryIterator(std::vector<std::vector<std::vector<Cell>>>& cells, bool end = false);

        std::unique_ptr<Particle>& operator*() const;
        BoundaryIterator& operator++();
        bool operator!=(const BoundaryIterator& other) const;

    private:
        std::vector<std::vector<std::vector<Cell>>>& cells;
        size_t x, y, z;
        std::list<std::unique_ptr<Particle>>::iterator particleIt;
        void advance();
    };

    BoundaryIterator beginBoundaryParticles();
    BoundaryIterator endBoundaryParticles();

    // Iterator for halo particles
    class HaloIterator {
    public:
        explicit HaloIterator(std::vector<std::vector<std::vector<Cell>>>& cells, bool end = false);

        std::unique_ptr<Particle>& operator*() const;
        HaloIterator& operator++();
        bool operator!=(const HaloIterator& other) const;

    private:
        std::vector<std::vector<std::vector<Cell>>>& cells;
        size_t x, y, z;
        std::list<std::unique_ptr<Particle>>::iterator particleIt;
        void advance();
    };

    HaloIterator beginHaloParticles();
    HaloIterator endHaloParticles();

private:
    // Method to initialize the grid based on the domain size and cutoff radius
    void initializeGrid();

    // Method to determine the cell type based on indices
    [[nodiscard]] CellType determineCellType(const std::array<size_t, 3>& indices) const;

    // Domain size
    std::array<double, 3> domainSize;

    // Cutoff radius
    double cutoffRadius;

    // Grid dimensions
    std::array<size_t, 3> gridDimensions;

    // 3D vector of cells
    std::vector<std::vector<std::vector<Cell>>> cells;
};