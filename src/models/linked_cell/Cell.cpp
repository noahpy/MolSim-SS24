
#include "Cell.h"

Cell::Cell()
    : type(CellType::Inner)
    , neighborCounter(0)
{
}

Cell::Cell(const CellType type)
    : type(type)
    , neighborCounter(0)
{
}

Cell::~Cell() = default;

void Cell::addParticle(Particle& particle)
{
    particles.emplace_back(particle);
}

void Cell::removeParticle(const Particle& particle)
{
    /*
     * It is necessary to use the pointer to be able to pass it to the lambda function
     * Intern the pointer is necessary because if we pass the particle object, it will be copied
     * Thus changing its memory address and the comparison will not work
     *
     * Before it was done like so: [particle](const std::reference_wrapper<particle>& p) { return &p.get() == &particle; }
     */
    const Particle* particlePtr = &particle;
    particles.remove_if(
        [particlePtr](const std::reference_wrapper<Particle>& p) { return &p.get() == particlePtr; });
}

[[nodiscard]] std::list<std::reference_wrapper<Particle>>& Cell::getParticles()
{
    return particles;
}

CellType Cell::getType() const
{
    return type;
}

std::list<std::reference_wrapper<Particle>>::iterator Cell::begin()
{
    return particles.begin();
}

std::list<std::reference_wrapper<Particle>>::iterator Cell::end()
{
    return particles.end();
}

int Cell::getCounter() const
{
    return neighborCounter;
}

void Cell::setCounter(int count)
{
    neighborCounter = count;
}
