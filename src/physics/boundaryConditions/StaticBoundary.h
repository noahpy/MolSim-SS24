
#pragma once
#include "models/linked_cell/CellGrid.h"
#include "physics/boundaryConditions/BoundaryCondition.h"
#include <map>

/**
 * @brief The StaticBoundary class is a class that represents a boundary condition where the boundary
 * is a wall consisting of particles
 */
class StaticBoundary : public BoundaryCondition {
public:
    /**
     *  @brief The constructor of the StaticBoundary
     * @param p_position The position of the boundary
     * @param cellGrid The cell grid to perform the static boundaries in
     */
    StaticBoundary(Position p_position, const CellGrid& cellGrid);

    /**
     * @brief Initialize the static boundary
     * @details This is necessary, as we only get the information the the size and type ot the side
     * once the simulation is initialized
     */
    void init();

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This will insert all particles of the static particles into the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * This will remove all particles of the static side from the halo cells
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
