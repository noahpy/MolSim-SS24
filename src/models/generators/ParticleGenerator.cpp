
#include "ParticleGenerator.h"
#include "models/linked_cell/CellGrid.h"
#include <spdlog/spdlog.h>

ParticleGenerator::ParticleGenerator(ParticleContainer& container)
    : container(container)
    , clusters(std::vector<std::unique_ptr<ParticleCluster>> {})
{
}

void ParticleGenerator::registerCluster(std::unique_ptr<ParticleCluster> cluster)
{
    clusters.push_back(std::move(cluster));
    spdlog::info("Registered cluster: {}", clusters.back()->toString());
}

void ParticleGenerator::generateClusters()
{
    size_t totalParticles = 0;
    for (const auto& cluster : clusters) {
        totalParticles += cluster->getTotalNumberOfParticles();
    }

    spdlog::info("Generating {} particles", totalParticles);

    std::vector<Particle> particles(totalParticles);
    size_t insertionIndex = 0;
    for (const auto& cluster : clusters) {
        cluster->generateCluster(particles, insertionIndex);
    }

    // Set the particles in the container
    container.particles = particles;
    // Initialize counter for active particles
    container.activeParticleCount = static_cast<int>(particles.size());

    // Clear the clusters
    clusters.clear();
}
