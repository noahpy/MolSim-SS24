
#include "WallBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"

WallBoundary::WallBoundary(
    Position p_position, const CellGrid& cellGrid, const CuboidParticleCluster& wallCluster)
    : BoundaryCondition(p_position)
    , wall()
{
    std::vector<Particle> generatedWall(wallCluster.getTotalNumberOfParticles());
    size_t insertIndex = 0;
    wallCluster.generateCluster(generatedWall, insertIndex);
    wall.particles = generatedWall;
    wall.activeParticleCount = static_cast<int>(generatedWall.size());

    for (Particle particle : generatedWall) {
        CellIndex haloIndexOfParticle = cellGrid.getIndexFromPos(particle.getX());
        // Check if wall particle is actually in a halo cell
        if (cellGrid.determineCellType(haloIndexOfParticle) != CellType::Halo)
            spdlog::warn(
                "Particle at position {}, {}, {} is assigned to be a wall particle at side {}, but "
                "is not in a halo cell.",
                particle.getX()[0],
                particle.getX()[1],
                particle.getX()[2],
                getPositionString(position));

        auto sides = coordinateToPosition(
            haloIndexOfParticle,
            cellGrid.getGridDimensions(),
            cellGrid.getGridDimensions()[2] > 1,
            true);
        // Check if wall particle is actually on the correct side
        if (std::find(sides.begin(), sides.end(), position) == sides.end())
            spdlog::warn(
                "Particle at position {}, {}, {} is assigned to be a wall particle at side {}, but "
                "is not at that side. It is assigned to cell {}, {}, {}",
                particle.getX()[0],
                particle.getX()[1],
                particle.getX()[2],
                getPositionString(position),
                haloIndexOfParticle[0],
                haloIndexOfParticle[1],
                haloIndexOfParticle[2]);

        std::reference_wrapper<Particle> particleRef(particle);
        particles.at(haloIndexOfParticle).emplace_back(particleRef);
    }
}

void WallBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    for (CellIndex haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        if (!LGDSim.getGrid()
                 .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                 ->getParticles()
                 .empty())
            spdlog::warn(
                "Wall at side {} would overwrite {} particles",
                getPositionString(position),
                LGDSim.getGrid()
                    .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                    ->getParticles()
                    .size());
        LGDSim.getGrid().cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->setParticles(
            particles[haloCellIndex]);
    }
}

void WallBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    for (CellIndex haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();
    }
}
