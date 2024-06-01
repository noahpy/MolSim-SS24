#pragma once

#include "CellType.h"
#include "models/Particle.h"
#include <functional>
#include <list>

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
    Cell();

    /**
     * @brief Constructor for Cell with a specified type.
     * @param type The type of the cell (boundary, halo, or bulk).
     */
    explicit Cell(CellType type);

    /** @brief Destructor for Cell. */
    ~Cell();

    /**
     * @brief Adds a particle to the cell.
     * @param particle A reference to the particle to be added.
     */
    void addParticle(Particle& particle);

    /**
     * @brief Removes a particle from the cell.
     * @param particle A reference to the particle to be removed.
     */
    void removeParticle(const Particle& particle);

    /**
     * @brief Returns a list of references to all particles in the cell.
     * @return A list of references to the particles in the cell.
     */
    [[nodiscard]] ParticleRefList& getParticles();

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

    /**
     * @brief Gets the current value of the neighbor counter.
     * @return The current value of the neighbor counter.
     */
    [[nodiscard]] int getCounter() const;

    /**
     * @brief Sets the value of the neighbor counter.
     * @param value The new value for the neighbor counter.
     */
    void setCounter(int value);

    /**
     * @brief Decrements the neighbor counter.
     */
    void decrementCounter() { neighborCounter--; };

    /**
     * @brief Increments the neighbor counter.
     */
    void incrementCounter() { neighborCounter++; };

private:
    /// The type of the cell (boundary, halo, or bulk).
    CellType type;

    /// A counter used for tracking processed neighbors (e.g., during force calculations).
    int neighborCounter;

    /// A list storing references to particles within the cell.
    ParticleRefList particles;
};
