#pragma once

#include "models/Particle.h"
#include "models/linked_cell/cell/CellType.h"
#include "utils/Position.h"
#include <functional>
#include <list>
#include <mutex>

/** @brief A 3D index representing a cell's position within the grid. */
typedef std::array<size_t, 3> CellIndex;
/** @brief A list of references to particles in a cell. */
typedef std::list<std::reference_wrapper<Particle>> ParticleRefList;

/** @class Cell
 *  @brief Represents a single cell within the CellGrid.
 *
 *  A Cell stores particles and tracks its type (boundary, halo, or bulk).
 *  It provides methods for managing particles, accessing their references,
 *  and managing a neighbor counter.
 */
class Cell {
public:
    /** @brief Default constructor for Cell, initializes it as a bulk cell. */
    Cell(const CellIndex index);

    /**
     * @brief Constructor for Cell with a specified type.
     * @param type The type of the cell (boundary, halo, or bulk).
     */
    explicit Cell(CellType type, const CellIndex index);

    /** @brief Copy constructor for Cell. */
    Cell(const Cell& other);

    /** @brief Destructor for Cell. */
    virtual ~Cell();

    /**
     * @brief Adds a particle to the cell.
     * @param particle A reference to the particle to be added.
     */
    void addParticle(Particle& particle);

    /**
     * @brief Removes a particle from the cell.
     * @param particle A reference to the particle to be removed.
     */
    void removeParticle(Particle& particle);

    /**
     * @brief Clears all particles from the cell.
     * @return void
     */
    void clearParticles();

    /**
     * @brief Returns a list of references to all particles in the cell.
     * @return A list of references to the particles in the cell.
     */
    [[nodiscard]] inline ParticleRefList& getParticles() { return particles; }

    /**
     * @brief Returns the type of the cell.
     * @return The CellType of the cell (boundary, halo, or bulk).
     */
    [[nodiscard]] CellType getType() const;

    /**
     * @brief Returns an iterator pointing to the beginning of the particle list.
     * @return An iterator to the beginning of the particle list.
     */
    ParticleRefList::iterator begin();

    /**
     * @brief Returns an iterator pointing to the end of the particle list.
     * @return An iterator to the end of the particle list.
     */
    ParticleRefList::iterator end();

    inline bool getVisited() const THREAD_SAFE
    {
        std::lock_guard<std::mutex> lock(mutex);
        return visited;
    }

    /**
     * @brief Returns the current state of the visited flag and sets to false if true.
     * @return The current state of the visited flag.
     */
    inline bool visit() THREAD_SAFE
    {
        std::lock_guard<std::mutex> lock(mutex);
        bool tmp = visited;
        visited = true;
        return tmp;
    }

    inline void unvisit() THREAD_SAFE
    {
        std::lock_guard<std::mutex> lock(mutex);
        visited = false;
    }

    /** @brief Index of the cell. */
    CellIndex myIndex;

    /** @brief Neighbours of the cell. */
    std::vector<std::pair<CellIndex, std::vector<Position>>> boundaryNeighbours;
    std::vector<std::pair<CellIndex, std::vector<Position>>> haloNeighbours;
    std::vector<std::pair<CellIndex, std::vector<Position>>> innerNeighbours;

    // Stencil neighbours
    std::vector<CellIndex> stencilNeighbours;

    // Forward declaration.
    class PairListIterator;

    /**
     * @brief Returns a PairIterator pointing to the beginning of the particle pairs.
     * @return A PairIterator at the beginning of the pairs.
     */
    PairListIterator beginPairs();

    /**
     * @brief Returns a PairIterator pointing to the end of the particle pairs.
     * @return A PairIterator at the end of the pairs.
     */
    PairListIterator endPairs();

private:
    /// The type of the cell (boundary, halo, or bulk).
    CellType type;

    /** @brief Flag to check if this cell has been visited. */
    bool visited = false;

    /// A list storing references to particles within the cell.
    ParticleRefList particles;

    /// mutex
    mutable std::mutex mutex;

public:
    class PairListIterator {
    public:
        /**
         * @brief Constructor for PairIterator.
         * @param particles A reference to the list of particles.
         * @param end Set to true to create an end iterator.
         */
        explicit PairListIterator(ParticleRefList& particles, bool end = false);

        /**
         * @brief Dereferences the iterator, returning a pair of particle pointers.
         * @return A pair of pointers to the current particle pair.
         */
        std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>> operator*()
            const;

        /**
         * @brief Increments the iterator to the next particle pair.
         * @return A reference to the incremented PairIterator.
         */
        PairListIterator& operator++();

        /**
         * @brief Checks if this iterator is not equal to another PairIterator.
         * @param other The other PairIterator to compare with.
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const PairListIterator& other) const;

    private:
        /// A reference to the list of particles to iterate over.
        ParticleRefList& particles;

        /// An iterator to the first particle in the current pair.
        ParticleRefList::iterator firstIt;

        /// An iterator to the second particle in the current pair.
        ParticleRefList::iterator secondIt;
    };
};
