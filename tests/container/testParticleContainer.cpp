
#include "Particle.h"
#include "ParticleContainer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

TEST(PContainerTests, enhancedForLoopParticles)
{
    // TODO implement test

    std::vector<Particle> particles { Particle {}, Particle {} };

    ParticleContainer container { particles };

    for (auto& p : container) {
        std::cout << p.getM() << std::endl;
    }

    EXPECT_EQ(true, true);
}
