
#include "Particle.h"
#include "ParticleContainer.h"
#include <gtest/gtest.h>
#include <ostream>
#include <vector>

// check if enhanced loop work as intended
TEST(PContainerTests, enhancedForLoopParticles)
{
    std::array<double, 3> zeros { 0, 0, 0 };

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

TEST(PContainerTests, pairIterators)
{
    std::array<double, 3> zeros { 0, 0, 0 };

    Particle p1 { zeros, zeros, 5, 0 };
    Particle p2 { zeros, zeros, 3, 0 };
    Particle p3 { zeros, zeros, 1, 0 };

    std::vector<Particle> particles { p1, p2, p3 };

    ParticleContainer container { particles };

    unsigned count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        std::cout << pt_pair.first.getM() << " | " << pt_pair.second.getM() << std::endl;
        ++count;
    }

    EXPECT_EQ(count, 3);

    container.addParticle(p1);
    container.addParticle(p1);

    count = 0;
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;
        std::cout << pt_pair.first.getM() << " | " << pt_pair.second.getM() << std::endl;
        ++count;
    }

    EXPECT_EQ(count, 10);
}
