
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
    particles.remove_if(
        [particle](const std::reference_wrapper<Particle>& p) { return &p.get() == &particle; });
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
