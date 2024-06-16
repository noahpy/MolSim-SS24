
#pragma once
#include "models/linked_cell/cell/Cell.h"
#include "physics/boundaryConditions/BoundaryCondition.h"
#include <spdlog/spdlog.h>

// forward declare
class LennardJonesDomainSimulation;

/**
 * @brief The SoftReflectiveBoundary class is a class that represents a boundary condition where
 * particles that leave the domain are reflected back into the domain about the boundary by
 * intorducing a mirror halo particle that forces the particle back
 */
class SoftReflectiveBoundary : public BoundaryCondition {
public:
    // lift constructor
    using BoundaryCondition::BoundaryCondition;

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This will introduce the halo particles
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * Checks if the particles have left the boundary cells and prevents them from beeing assigned
     * to the halo cells
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation) override;

private:
    /**
     * @brief Filter the halo neighbors to find the one that is at the side we want. This should
     * ignore diagonals
     * @param haloNeighbors The halo neighbors to filter
     * @return The relevant halo neighbor
     */
    CellIndex filterHaloNeighbors(
        const std::vector<std::pair<CellIndex, std::vector<Position>>>& haloNeighbors)
    {
        for (auto& neighbors : haloNeighbors) {
            auto cellIndex = neighbors.first;
            auto positions = neighbors.second;

            for (auto& position : positions) {
                if (position == this->position && positions.size() == 1) {
                    // Only if the neighbor is at the side we want, then we can use it
                    return cellIndex;
                }
            }
        }
        spdlog::error("No suitable halo neighbor found for boundary handling. This is a bug");
        return { 0, 0, 0 };
    };

    std::array<double, 3> getPointOnBoundaryPlane(const LennardJonesDomainSimulation& LJDSim);

    std::vector<Particle>
        insertedParticles; /**< The particles that were inserted. These will be recycled */
    size_t insertionIndex; /**< The index of the next particle to recycle */

    double repulsiveDistance = -1; /**< The distance to the boundary where the repulsion starts */
};
