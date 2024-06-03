
#include "simulation/LennardJonesDomainSimulation.h"
#include <spdlog/spdlog.h>

class SoftReflectiveBoundary : public BoundaryCondition {
public:
    using BoundaryCondition::BoundaryCondition;

    void preUpdateBoundaryHandling(Simulation& simulation) override;
    void postUpdateBoundaryHandling(Simulation& simulation) override {
        // Do nothing
    };

private:
    CellIndex filterHaloNeighbors(std::vector<std::pair<CellIndex, std::vector<Position>>> haloNeighbors) {
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
        spdlog::warn("No suitable halo neighbor found for boundary handling. This is a bug");
        return { 0, 0, 0 };
    };

    std::vector<Particle> insertedParticles;
    size_t insertionIndex;
};
