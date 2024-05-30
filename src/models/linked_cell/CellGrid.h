
#pragma once

#include "Cell.h"
#include "CellType.h"
#include "models/ParticleContainer.h"
#include <array>
#include <memory>
#include <vector>

typedef std::vector<std::vector<std::vector<std::unique_ptr<Cell>>>> CellVec;

/** @class CellGrid
 *  @brief Represents a 3D grid of cells to organize particles in a simulation.
 *
 *  This class provides methods for adding particles, identifying neighboring particles,
 *  and iterating over boundary and halo cells. It also internally manages the domain size,
 *  cutoff radius, and grid dimensions.
 */
class CellGrid {
public:
    /**
     * @brief Constructor for CellGrid.
     * @param domainSize A 3D array representing the size of the simulation domain.
     * @param cutoffRadius The cutoff radius for particle interactions.
     */
    CellGrid(const std::array<double, 3>& domainSize, double cutoffRadius);

    /** @brief Destructor for CellGrid. */
    ~CellGrid();

    /**
     * @brief Adds a particle to the appropriate cell in the grid.
     * @param particle The particle to be added.
     */
    void addParticle(Particle& particle);

    /**
     * @brief Adds all particles from a ParticleContainer to the grid.
     * @param particleContainer The container holding the particles to be added.
     */
    void addParticlesFromContainer(ParticleContainer& particleContainer);

    /**
     * @brief Returns all particle references of a given cell and its neighbors
     * @param cellIndex The index of the target cell
     * @return A list of references to all particles in the target cell and its neighbors
     */
    [[nodiscard]] std::list<std::reference_wrapper<Particle>> getNeighboringParticles(
        const std::array<size_t, 3>& cellIndex);

    // Forward declaration (see bottom)
    class BoundaryIterator;

    // Forward declaration (see bottom)
    class HaloIterator;

    /** @brief Returns an iterator to the beginning of boundary particles. */
    BoundaryIterator beginBoundaryParticles();

    /** @brief Returns an iterator to the end of boundary particles. */
    BoundaryIterator endBoundaryParticles();

    /** @brief Returns an iterator to the beginning of halo particles. */
    HaloIterator beginHaloParticles();

    /** @brief Returns an iterator to the end of halo particles. */
    HaloIterator endHaloParticles();

protected:
    /**
     * @brief Initializes the grid structure based on the domain size and cutoff radius.
     */
    void initializeGrid();

    /**
     * @brief Determines the cell type (boundary, halo, or bulk) based on its indices.
     * @param indices The 3D index of the cell.
     * @return The CellType of the specified cell.
     */
    [[nodiscard]] CellType determineCellType(const std::array<size_t, 3>& indices) const;

private:
    /// The size of the simulation domain in each dimension.
    std::array<double, 3> domainSize;

    /// The cutoff radius for particle interactions.
    double cutoffRadius;

    /// The dimensions of the cell grid in each dimension.
    std::array<size_t, 3> gridDimensions;

    /// The 3D vector storing the cells in the grid.
    CellVec cells;

    /// A vector storing the indices of boundary cells.
    std::vector<CellIndex> boundaryCells;

    /// A vector storing the indices of halo cells.
    std::vector<CellIndex> haloCells;

/* ##### Detailed Iterator Definitions ##### */
public:
    /** @class BoundaryIterator
     * @brief Iterator for iterating over boundary cells.
     */
    class BoundaryIterator {
    public:
        /**
         * @brief Constructor for BoundaryIterator.
         * @param boundaries A reference to the vector of boundary cell indices.
         * @param end Set to true to create an end iterator.
         */
        explicit BoundaryIterator(std::vector<CellIndex>& boundaries, bool end = false);

        /**
         * @brief Dereferences the iterator, returning the current CellIndex.
         * @return The CellIndex of the current boundary cell.
         */
        CellIndex operator*() const;

        /**
         * @brief Increments the iterator to the next boundary cell.
         * @return A reference to the incremented BoundaryIterator.
         */
        BoundaryIterator& operator++();

        /**
         * @brief Checks if this iterator is not equal to another BoundaryIterator.
         * @param other The other BoundaryIterator to compare with.
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const BoundaryIterator& other) const;

    private:
        /// The current index in the boundary cell vector.
        size_t index;

        /// A reference to the vector of boundary cell indices.
        std::vector<CellIndex>& boundaries;
    };

    /** @class HaloIterator
     * @brief Iterator for iterating over halo cells.
     */
    class HaloIterator {
    public:
        /**
         * @brief Constructor for HaloIterator.
         * @param halos A reference to the vector of halo cell indices.
         * @param end Set to true to create an end iterator.
         */
        explicit HaloIterator(std::vector<CellIndex>& halos, bool end = false);

        /**
         * @brief Dereferences the iterator, returning the current CellIndex.
         * @return The CellIndex of the current halo cell.
         */
        CellIndex operator*() const;

        /**
         * @brief Increments the iterator to the next halo cell.
         * @return A reference to the incremented HaloIterator.
         */
        HaloIterator& operator++();

        /**
         * @brief Checks if this iterator is not equal to another HaloIterator.
         * @param other The other HaloIterator to compare with.
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const HaloIterator& other) const;

    private:
        /// A reference to the vector of halo cell indices.
        std::vector<CellIndex>& halos;

        /// The current index in the halo cell vector.
        size_t index;
    };

    // PairIterator for list of particle pointers
    class PairIterator {
    public:
        /**
         * @brief Constructor for PairIterator.
         * @param particles A reference to the list of particles.
         * @param end Set to true to create an end iterator.
         */
        explicit PairIterator(
            std::list<std::reference_wrapper<Particle>>& particles, bool end = false);

        /**
         * @brief Dereferences the iterator, returning a pair of particle pointers.
         * @return A pair of pointers to the current particle pair.
         */
        std::pair<Particle*, Particle*> operator*() const;

        /**
         * @brief Increments the iterator to the next particle pair.
         * @return A reference to the incremented PairIterator.
         */
        PairIterator& operator++();

        /**
         * @brief Checks if this iterator is not equal to another PairIterator.
         * @param other The other PairIterator to compare with.
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const PairIterator& other) const;

        /**
         * @brief Returns a PairIterator pointing to the beginning of the particle pairs.
         * @param particles A reference to the list of particles.
         * @return A PairIterator at the beginning of the pairs.
         */
        static PairIterator beginPairs(std::list<std::reference_wrapper<Particle>>& particles);

        /**
         * @brief Returns a PairIterator pointing to the end of the particle pairs.
         * @param particles A reference to the list of particles.
         * @return A PairIterator at the end of the pairs.
         */
        static PairIterator endPairs(std::list<std::reference_wrapper<Particle>>& particles);

    private:
        /// A reference to the list of particles to iterate over.
        std::list<std::reference_wrapper<Particle>>& particles;

        /// An iterator to the first particle in the current pair.
        std::list<std::reference_wrapper<Particle>>::iterator firstIt;

        /// An iterator to the second particle in the current pair.
        std::list<std::reference_wrapper<Particle>>::iterator secondIt;

        /**
         * @brief Advances the iterators to the next valid particle pair.
         */
        void advance();
    };
};
