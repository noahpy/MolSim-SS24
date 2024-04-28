/*
 *ParticleContainer Class Implementation
 */

#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(const std::vector<Particle>& particles)
{
    this->particles = particles;
}

// Adds a particle to the container
void ParticleContainer::addParticle(const Particle& p)
{
    particles.push_back(p);
}

// Iterator for single particles
std::vector<Particle>::iterator ParticleContainer::begin()
{
    return particles.begin();
}
std::vector<Particle>::iterator ParticleContainer::end()
{
    return particles.end();
}

// Returns an iterator to the first pair of particles
ParticleContainer::PairIterator ParticleContainer::beginPairs()
{
    return { particles.begin(), particles.begin(), particles.end() };
}

// Returns an iterator indicating the end of pairs
ParticleContainer::PairIterator ParticleContainer::endPairs()
{
    return { particles.begin(), particles.end() - 1, particles.end() };
}

// PairIterator constructor
ParticleContainer::PairIterator::PairIterator(
    std::vector<Particle>::iterator start,
    std::vector<Particle>::iterator first,
    std::vector<Particle>::iterator last)
    : start(start)
    , first(first)
    , second(first)
    , last(last)
{
    if (start != last)
        ++second;
}

// Returns a reference to the current pair
std::pair<Particle&, Particle&> ParticleContainer::PairIterator::operator*() const
{
    return { *first, *second };
}

// Advances the iterator to the next pair
ParticleContainer::PairIterator& ParticleContainer::PairIterator::operator++()
{
    if (second == last - 1) {
        // In case of last pair: return the last pair without change
        if (first == last - 1)
            return *this;
        ++first;
        second = first+1;
        return *this;
    }
    ++second;
    return *this;
}

// Compares two iterators to determine if they are not equal
bool ParticleContainer::PairIterator::operator!=(const PairIterator& other) const
{
    return first != other.first || second != other.second;
}
