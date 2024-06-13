/*
 *ParticleContainer Class Implementation
 */

#include "ParticleContainer.h"
#include "Particle.h"
#include <algorithm>

ParticleContainer::ParticleContainer()
{
    this->particles = std::vector<Particle> {};
    activeParticleCount = 0;
}

ParticleContainer::ParticleContainer(const std::vector<Particle>& particles)
{
    this->particles = particles;

    activeParticleCount = 0;
    for (auto& p : particles) {
        if (p.getActivity() == true) {
            activeParticleCount++;
        }
    }

}

// Adds a particle to the container
void ParticleContainer::addParticle(const Particle& p)
{
    particles.push_back(p);
    activeParticleCount++;
}

void ParticleContainer::removeParticle(Particle& p)
{
    p.setV({ 0, 0, 0 });
    p.setOldF({ 0, 0, 0 });
    p.setF({ 0, 0, 0 });
    p.setActivity(false);
    activeParticleCount--;
}

std::vector<Particle> ParticleContainer::getContainer() const
{
    return particles;
}

// ActiveIterator Implementation
ParticleContainer::ActiveIterator ParticleContainer::begin()
{
    return { particles.begin(), particles.end() };
}

ParticleContainer::ActiveIterator ParticleContainer::end()
{
    return { particles.end(), particles.end() };
}


void ParticleContainer::ActiveIterator::advanceToNextActive()
{
    while (current != end && !current->getActivity()) {
        ++current;
    }
}

ParticleContainer::ActiveIterator::ActiveIterator(std::vector<Particle>::iterator start, std::vector<Particle>::iterator end)
    : begin(start), current(start), end(end)
{
    advanceToNextActive();
}

Particle& ParticleContainer::ActiveIterator::operator*() const
{
    return *current;
}

ParticleContainer::ActiveIterator& ParticleContainer::ActiveIterator::operator++()
{
    ++current;
    advanceToNextActive();
    return *this;
}

ParticleContainer::ActiveIterator& ParticleContainer::ActiveIterator::operator--()
{
    do {
        --current;
    } while (current != begin && !current->getActivity());
    return *this;
}


bool ParticleContainer::ActiveIterator::operator!=(const ActiveIterator& other) const
{
    return current != other.current;
}

bool ParticleContainer::ActiveIterator::operator==(const ActiveIterator& other) const
{
    return current == other.current;
}

//PairIterator Implementation
// Returns an iterator to the first pair of particles
ParticleContainer::PairIterator ParticleContainer::beginPairs()
{
    if (particles.empty()) {
        return { begin(), begin(), end() };
    }
    return { begin(), begin(), --end() };
}

// Returns an iterator indicating the end of pairs
ParticleContainer::PairIterator ParticleContainer::endPairs()
{
    if (particles.empty()) {
        return { begin(), end(), end() };
    }
    return { begin(), --end(), --end() };
}

// PairIterator constructor
ParticleContainer::PairIterator::PairIterator(
    ActiveIterator start,
    ActiveIterator first,
    ActiveIterator last)
    : start(start)
    , first(first)
    , second(first)
    , last(last)
{
    if (start != last)
        ++second;
}

Particle& ParticleContainer::PairIterator::getLast()
{
    return *last;
}

// Returns a reference to the current pair
std::pair<Particle&, Particle&> ParticleContainer::PairIterator::operator*() const
{
    return { *first, *second };
}

// Advances the iterator to the next pair
ParticleContainer::PairIterator& ParticleContainer::PairIterator::operator++()
{
    // inc second if possible, else inc first and reset second
    if (second == last) {
        // In case of last pair: return the last pair without change
        if (first == last)
            return *this;
        ++first;
        second = first;
        ++second;
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

bool ParticleContainer::PairIterator::operator==(const PairIterator& other) const
{
    return first == other.first && second == other.second;
}
