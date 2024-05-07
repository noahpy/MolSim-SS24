
#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include <gtest/gtest.h>
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
        for (std::pair<Particle, Particle> particle_pair : pairs) {
            Particle pp1 = particle_pair.first;
            Particle pp2 = particle_pair.second;
            EXPECT_FALSE(
                pt_pair.first.getM() == pp1.getM() && pt_pair.second.getM() == pp2.getM() ||
                pt_pair.first.getM() == pp2.getM() && pt_pair.second.getM() == pp1.getM())
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
        for (std::pair<Particle, Particle> particle_pair : pairs) {
            Particle pp1 = particle_pair.first;
            Particle pp2 = particle_pair.second;
            EXPECT_FALSE(
                pt_pair.first.getM() == pp1.getM() && pt_pair.second.getM() == pp2.getM() ||
                pt_pair.first.getM() == pp2.getM() && pt_pair.second.getM() == pp1.getM())
                << "Found duplicate pair!";
        }
        pairs.emplace_back(pt_pair);
        ++count;
    }

    EXPECT_EQ(count, 10);
}

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
