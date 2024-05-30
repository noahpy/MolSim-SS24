
#pragma once

#include "Cell.h"
#include "CellType.h"
#include "models/ParticleContainer.h"
#include <array>
#include <memory>
#include <vector>

typedef std::vector<std::vector<std::vector<std::unique_ptr<Cell>>>> CellVec;

// Class representing the grid of cells
class CellGrid {
public:
    // Constructor
    CellGrid(const std::array<double, 3>& domainSize, double cutoffRadius);

    // Destructor
    ~CellGrid();

    // Method to add a particle to the grid
    void addParticle(Particle& particle);

    // Method to add particles from ParticleContainer
    void addParticlesFromContainer(ParticleContainer& particleContainer);

    // Create a list of all particle pointers of a cell and it's neighbors
    [[nodiscard]] std::list<std::reference_wrapper<Particle>> getNeighboringParticles(
        const std::array<size_t, 3>& cellIndex);

    // Iterator for boundary particles
    class BoundaryIterator {
    public:
        explicit BoundaryIterator(std::vector<Cell>& boundaryCells, bool end = false);

        std::unique_ptr<Particle>& operator*() const;
        BoundaryIterator& operator++();
        bool operator!=(const BoundaryIterator& other) const;

    private:
        std::vector<Cell>& boundaryCells;
        size_t cellIndex;
        std::list<std::unique_ptr<Particle>>::iterator particleIt;
        void advance();
    };

    BoundaryIterator beginBoundaryParticles();
    BoundaryIterator endBoundaryParticles();

    // Iterator for halo particles
    class HaloIterator {
    public:
        explicit HaloIterator(std::vector<Cell>& haloCells, bool end = false);

        std::unique_ptr<Particle>& operator*() const;
        HaloIterator& operator++();
        bool operator!=(const HaloIterator& other) const;

    private:
        std::vector<Cell>& haloCells;
        size_t cellIndex;
        std::list<std::unique_ptr<Particle>>::iterator particleIt;
        void advance();
    };

    HaloIterator beginHaloParticles();
    HaloIterator endHaloParticles();

    // PairIterator for list of particle pointers
    class PairIterator {
    public:
        // Constructor
        explicit PairIterator(
            std::list<std::reference_wrapper<Particle>>& particles, bool end = false);

        // Dereference operator
        std::pair<Particle*, Particle*> operator*() const;

        // Increment operator
        PairIterator& operator++();

        // Inequality operator
        bool operator!=(const PairIterator& other) const;

        // Functions to get the beginning and end pair iterators
        static PairIterator beginPairs(std::list<std::reference_wrapper<Particle>>& particles);
        static PairIterator endPairs(std::list<std::reference_wrapper<Particle>>& particles);

    private:
        // Reference to the list of particles
        std::list<std::reference_wrapper<Particle>>& particles;

        // Iterators to manage the current pair
        std::list<std::reference_wrapper<Particle>>::iterator firstIt;
        std::list<std::reference_wrapper<Particle>>::iterator secondIt;

        // Method to advance the iterator to the next pair
        void advance();
    };

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
    CellVec cells;

    // Vectors to store boundary and halo cells
    std::vector<CellIndex> boundaryCells;
    std::vector<CellIndex> haloCells;
};
