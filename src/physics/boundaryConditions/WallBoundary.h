
#pragma once
#include "models/generators/CuboidParticleCluster.h"
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
     * @param wall The CuboidParticleCluster that makes up the wall
     */
    WallBoundary(Position p_position, const CellGrid& cellGrid, const CuboidParticleCluster& wall);

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This will insert all particles of the wall particles into the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * This will remove all particles of the wall side from the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation) override;

private:
    std::map<CellIndex, ParticleRefList>
        particles; /**< Map to store the particles for all halo cells */
    ParticleContainer wall;
};
