

#include "models/ParticleContainer.h"
#include <functional>

class PhysicsStrategy {
public:
    PhysicsStrategy(
        std::function<void(ParticleContainer)> calX,
        std::function<void(ParticleContainer)> calV,
        std::function<void(ParticleContainer)> calF)
    {
        this->calX = calX;
        this->calV = calV;
        this->calF = calF;
    }

    std::function<void(ParticleContainer)> calX;
    std::function<void(ParticleContainer)> calV;
    std::function<void(ParticleContainer)> calF;
};
