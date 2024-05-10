
#include "ParticleGenerator.h"
#include "utils/ArrayUtils.h"
#include <spdlog/spdlog.h>

ParticleGenerator::ParticleGenerator(ParticleContainer& container)
    : container(container)
    , clusters(std::vector<CuboidParticleCluster> {})
{
}

void ParticleGenerator::registerCluster(const CuboidParticleCluster& cluster)
{
    clusters.push_back(cluster);
    spdlog::info("Registered cluster: {}", cluster.toString());
}

void ParticleGenerator::generateClusters()
{
    size_t totalParticles = 0;
    for (const auto& cluster : clusters) {
        totalParticles += cluster.getTotalNumberOfParticles();
    }

    spdlog::info("Generating {} particles", totalParticles);

    std::vector<Particle> particles(totalParticles);
    size_t insertionIndex = 0;
    for (const auto& cluster : clusters) {
        cluster.generateCluster(particles, insertionIndex);
    }

    // Set the particles in the container
    container.particles = particles;

    // Clear the clusters
    clusters.clear();
}
