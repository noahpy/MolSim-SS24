
#pragma once

#include "models/ParticleContainer.h"
#include "models/linked_cell/cell/Cell.h"
#include "models/linked_cell/cell/CellType.h"
#include <array>
#include <memory>
#include <vector>

/** @brief A 3D vector of pointers to Cells. */
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
     * @param domainOrigin 3D array representing the "origin" of the simulation domain.
     * @param domainSize A 3D array representing the size of the simulation domain.
     * @param cutoffRadius The cutoff radius for particle interactions.
     */
    CellGrid(
        std::array<double, 3> domainOrigin, std::array<double, 3> domainSize, double cutoffRadius);

    /** @brief Destructor for CellGrid. */
    ~CellGrid();

    /// The 3D vector storing the cells in the grid.
    CellVec cells;

    // The bottom-left-back point of the domain
    std::array<double, 3> domainOrigin;
    // The bottom-right-back point of the domain
    std::array<double, 3> domainEnd;
    /// The dimensionality of the gird (either 2 or 3)
    size_t gridDimensionality;

    /// The cutoff radius for particle interactions.
    double cutoffRadius;

    /// The square of the cutoff radius
    double cutoffRadiusSquared;

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
     * @brief Returns the index to all neighbours,
     *        which have not been paired up to the specified cell.
     *        It also saves and resets the forces of the specified cell and its
     *        neighbout cells, so that one can simply add the new forces to the particles.
     * @param cellIndex The index of the target cell
     * @return A list of references to all particles in the target cell and its neighbors
     */
    [[nodiscard]] std::list<CellIndex> getNeighbourCells(const CellIndex& cellIndex) const;

    /**
     * @brief Resets the forces for all particles for the next force calculation
     * @param particleContainer The container holding the particles
     * @return void
     */
    void preCalcSetup(ParticleContainer& particleContainer) const;

    /**
     * @brief Resets the visited flag of all cells to false
     * @return void
     */
    void postCalcSetup() const;

    /**
     * @brief Updates the cell lists by the position of all particles.
     */
    void updateCells();

    // Forward declaration (see bottom)
    class BoundaryIterator;

    // Forward declaration (see bottom)
    class HaloIterator;

    /**
     * @brief Returns an iterator to the beginning of boundary particles.
     * @param position The position of the boundary.
     * @return A BoundaryIterator for the given position
     */
    BoundaryIterator boundaryCellIterator(Position position) const;

    /**
     * @brief Returns an iterator to the beginning of halo particles.
     * @param position The position of the boundary.
     * @return A HaloIterator for the given position
     */
    HaloIterator haloCellIterator(Position position) const;

    /* @brief Sets the domain origin.
     * @param domainOrigin 3D array representing the "origin" of the simulation domain.
     * */
    void setDomainOrigin(const std::array<double, 3>& domainOrigin);

    /* @brief Sets the domain size.
     * @param domainSize A 3D array representing the size of the simulation domain.
     * */
    void setDomainSize(const std::array<double, 3>& domainSize);

    /* @brief Sets the cutoff radius.
     * @param cutoffRadius The cutoff radius for particle interactions.
     * */
    void setCutoffRadius(double cutoffRadius);

    /**
     * @brief Returns the domain origin.
     * @return The domain origin.
     * */
    inline std::array<double, 3> getDomainOrigin() const { return domainOrigin; }

    /**
     * @brief Returns the domain size.
     * @return The domain size.
     * */
    inline std::array<double, 3> getDomainSize() const { return domainSize; }

    /**
     * @brief Get the grids dimensions
     * @return The grids dimensions
     */
    inline std::array<size_t, 3> getGridDimensions() const { return gridDimensions; }

    /**
     * @brief Returns the cutoff radius.
     * @return The cutoff radius.
     * */
    inline double getCutoffRadius() const { return cutoffRadius; }

    /**
     * @brief Returns the index of the cell containing the specified position.
     * @param pos The 3D position of the cell.
     * @return The 3D index of the cell containing the specified position.
     */
    CellIndex getIndexFromPos(const std::array<double, 3>& pos) const;

    /**
     * @brief Determines the cell type (boundary, halo, or bulk) based on its indices.
     * @param indices The 3D index of the cell.
     * @return The CellType of the specified cell.
     */
    [[nodiscard]] CellType determineCellType(const std::array<size_t, 3>& indices) const;

protected:
    /**
     * @brief Initializes the grid structure based on the domain size and cutoff radius.
     */
    void initializeGrid();

    /**
     * @brief Determines the neighbours of the specified cell.
     * @param cell The index of the cell to determine the neighbours of.
     */
    void determineNeighbours(CellIndex cell);

private:
    /// The size of the simulation domain in each dimension.
    std::array<double, 3> domainSize;

    /// The size of a cell according to the domain size.
    std::array<double, 3> cellSize;

    /// The dimensions of the cell grid in each dimension.
    std::array<size_t, 3> gridDimensions;

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
        BoundaryIterator(Position position, std::array<size_t, 3> gridDimensions, bool is2D);

        /**
         * @brief Returns an iterator to the beginning of boundary cells of the given position (for
         * range based loop)
         * @return The beginning BoundaryIterator
         */
        BoundaryIterator begin();
        /**
         * @brief Returns an iterator to the end of boundary cells of the given position (for range
         * based loop)
         * @return The end BoundaryIterator
         */
        BoundaryIterator end();

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
        /// The current index in the boundaries vector.
        size_t index;

        /// The relevant boundaries to consider
        std::vector<CellIndex> boundaries;

        /// The dimensions of the grid
        std::array<size_t, 3> gridDimensions;

        bool is2D; /**< True if the grid is 2D, false if 3D. */
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
        HaloIterator(Position position, std::array<size_t, 3> gridDimensions, bool is2D);

        /**
         * @brief Returns an iterator to the beginning of halo cells of the given position (for
         * range based loop)
         * @return The beginning HaloIterator
         */
        HaloIterator begin();
        /**
         * @brief Returns an iterator to the end of halo cells of the given position (for range
         * based loop)
         * @return The end HaloIterator
         */
        HaloIterator end();

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
        /// The relevant boundaries to consider
        std::vector<CellIndex> boundaries;

        std::array<size_t, 3> gridDimensions;

        /// The current index in the halo cell vector.
        size_t index;

        bool is2D; /**< True if the grid is 2D, false if 3D. */
    };
};
