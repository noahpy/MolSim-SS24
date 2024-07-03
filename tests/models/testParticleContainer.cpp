
#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "models/generators/CuboidParticleCluster.h"
#include "models/generators/ParticleGenerator.h"
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <vector>

std::array<double, 3> zeros { 0, 0, 0 };
std::array<double, 3> ones { 1, 1, 1 };

// check if enhanced loop work as intended
TEST(PContainerTests, particleIterate)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    double result = 0;
    for (auto& p : container) {
        result += p.getM();
    }

    EXPECT_EQ(result, 9);
}

// check if modifying F works
TEST(PContainerTests, particleItModF)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    double result = 0;
    for (auto& p : container) {
        result += p.getF()[0];
        p.setF(ones);
    }

    EXPECT_EQ(result, 0);

    result = 0;
    for (auto& p : container) {
        result += p.getF()[0];
    }

    EXPECT_EQ(result, 3);
}

// check if modifying X works
TEST(PContainerTests, particleItModX)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    double result = 0;
    for (auto& p : container) {
        result += p.getX()[0];
        p.setX(ones);
    }

    EXPECT_EQ(result, 0);

    result = 0;
    for (auto& p : container) {
        result += p.getX()[0];
    }

    EXPECT_EQ(result, 3);
}

// check if modifying V works
TEST(PContainerTests, particleItModV)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    double result = 0;
    for (auto& p : container) {
        result += p.getV()[0];
        p.setV(ones);
    }

    EXPECT_EQ(result, 0);

    result = 0;
    for (auto& p : container) {
        result += p.getV()[0];
    }

    EXPECT_EQ(result, 3);
}

// check if empty particle container works
TEST(PContainerTests, particleItZero)
{
    std::vector<Particle> particles {};

    ParticleContainer container { particles };

    unsigned count = 0;
    for (auto& p : container) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}

// check if distinct pair iteration works
TEST(PContainerTests, pairIterators)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };
    Particle p4 { zeros, zeros, 9, 0 };
    Particle p5 { zeros, zeros, 8, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    std::vector<std::pair<Particle, Particle>> pairs;

    unsigned count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        for (const std::pair<Particle, Particle>& particle_pair : pairs) {
            EXPECT_FALSE(
                pt_pair.first.getM() == particle_pair.first.getM() &&
                    pt_pair.second.getM() == particle_pair.second.getM() ||
                pt_pair.first.getM() == particle_pair.second.getM() &&
                    pt_pair.second.getM() == particle_pair.first.getM())
                << "Found duplicate pair!";
        }
        pairs.emplace_back(pt_pair);
        ++count;
    }

    EXPECT_EQ(count, 3);

    container.addParticle(p4);
    container.addParticle(p5);

    pairs.clear();
    count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        for (const std::pair<Particle, Particle>& particle_pair : pairs) {
            EXPECT_FALSE(
                pt_pair.first.getM() == particle_pair.first.getM() &&
                    pt_pair.second.getM() == particle_pair.second.getM() ||
                pt_pair.first.getM() == particle_pair.second.getM() &&
                    pt_pair.second.getM() == particle_pair.first.getM())
                << "Found duplicate pair!";
        }
        pairs.emplace_back(pt_pair);
        ++count;
    }

    EXPECT_EQ(count, 10);

    container.particles[2].setActivity(false);

    pairs.clear();
    count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        for (const std::pair<Particle, Particle>& particle_pair : pairs) {
            EXPECT_FALSE(
                pt_pair.first.getM() == particle_pair.first.getM() &&
                    pt_pair.second.getM() == particle_pair.second.getM() ||
                pt_pair.first.getM() == particle_pair.second.getM() &&
                    pt_pair.second.getM() == particle_pair.first.getM())
                << "Found duplicate pair!";
        }
        pairs.emplace_back(pt_pair);
        ++count;
    }

    EXPECT_EQ(count, 6);
}

// check if distinct pair iteration works with none / one particle in container
TEST(PContainerTests, pairItZero)
{
    std::vector<Particle> particles {};

    ParticleContainer container { particles };

    unsigned count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        ++count;
    }

    EXPECT_EQ(count, 0);

    Particle p1 { zeros, zeros, 5, 0 };

    container.addParticle(p1);

    count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        ++count;
    }

    EXPECT_EQ(count, 0);
}

// Check if active particle iteration works
TEST(PContainerTests, particleActiveIterate)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };
    container.particles[1].setActivity(false);

    double result = 0;
    for (auto& p : container) {
        result += p.getM();
    }

    EXPECT_EQ(result, 6);

    container.particles[1].setActivity(true);
    container.particles[0].setActivity(false);

    result = 0;
    for (auto& p : container) {
        result += p.getM();
    }

    EXPECT_EQ(result, 4);

    container.particles[0].setActivity(true);
    container.particles[2].setActivity(false);

    result = 0;
    for (auto& p : container) {
        result += p.getM();
    }

    EXPECT_EQ(result, 8);
}

// check if modifying F works with the active particle iteration
TEST(PContainerTests, particleActiveItModF)
{
    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };
    container.particles[0].setActivity(false);

    double result = 0;
    for (auto& p : container) {
        result += p.getF()[0];
        p.setF(ones);
    }

    EXPECT_EQ(result, 0);

    result = 0;
    for (auto& p : container) {
        result += p.getF()[0];
    }

    EXPECT_EQ(result, 2);
}

// check if empty particle container works for the activity iteration
TEST(PContainerTests, particleActiveItZero)
{
    std::vector<Particle> particles {};

    ParticleContainer container { particles };

    unsigned count = 0;
    for (auto& p : container) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}

// check if particle ids are set coorectly
TEST(PContainerTests, particleIdTest)
{
    std::vector<Particle> particles {};

    ParticleContainer container { particles }; 

    // add per clusters
    ParticleGenerator generator(container);
    generator.registerCluster(std::make_unique<CuboidParticleCluster>(
        CuboidParticleCluster({ 2, 2, 4 }, 2, 3, 2, 0.4, 4, { 0, 0, 0 }, 0, 0, 0)));

    generator.registerCluster(std::make_unique<SphereParticleCluster>(
        SphereParticleCluster({ 2, 2, 4 }, 4, 2, 0.3, 7, zeros, 0, 0)));

    generator.generateClusters();

     // add Particles manually
    container.addParticle({ zeros, zeros, 5 });
    container.addParticle({ zeros, zeros, 3 });
    container.addParticle({ zeros, zeros, 1 });

    for(size_t i=0; i < container.particles.size(); i++){
        EXPECT_EQ(container.particles[i].getID(), i) << "Mass : " << container.particles[i].getM();
    }



}
