
#pragma once
#include "models/linked_cell/CellGrid.h"
#include "physics/boundaryConditions/BoundaryCondition.h"
#include <map>

/**
 * @brief The WallBoundary class is a class that represents a boundary condition where the boundary
 * is a wall consisting of particles
 */
class WallBoundary : public BoundaryCondition {
public:
    /**
     *  @brief The constructor of the WallBoundary
     * @param p_position The position of the boundary
     * @param cellGrid The cell grid to perform the wall boundaries in
     */
    WallBoundary(Position p_position, const CellGrid& cellGrid);

    /**
     * @brief Initialize the wall boundary with a thickness and a particle
     * @details This is necessary, as we only get the information the the size and type ot the wall
     * once the simulation is initialized
     * @param thickness The thickness of the wall in particles
     * @param particle The particle to use as the wall
     */
    void init(size_t thickness, const Particle& particle);

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This will insert all particles of the walls into the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * This will remove all particles of the walls from the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation) override;

private:
    bool isInitialized = false; /**< Flag to check if the wall is initialized */
    const CellGrid& cellGrid; /**< The cell grid to perform the wall boundaries in */
    std::map<CellIndex, ParticleRefList>
        particles; /**< Map to store the particles for all halo cells */

    inline void checkInitialized() const; /**< Check if the wall is initialized */
};
