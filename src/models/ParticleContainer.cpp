/*
 *ParticleContainer Class Implementation
 */

#include "ParticleContainer.h"
#include "Particle.h"

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
        if (p.getActivity()) {
            activeParticleCount++;
        }
    }
}

// Adds a particle to the container
void ParticleContainer::addParticle(const Particle& p)
{
    particles.push_back(p);
    particles.back().setID(particles.size() - 1);
    activeParticleCount++;
}

void ParticleContainer::removeParticle(Particle& p)
{
    p.setActivity(false);
    if (activeParticleCount) {
        activeParticleCount--;
        inactiveParticleMap.insert_or_assign(p.getID(), p.getID());
    }
}

size_t ParticleContainer::getIndex(size_t id)
{
    auto lowerBound = inactiveParticleMap.lower_bound(0);
    auto upperBound = inactiveParticleMap.upper_bound(id);
    return id - std::distance(lowerBound, upperBound);
}

std::vector<Particle> ParticleContainer::getContainer() const
{
    return particles;
}

// ActiveIterator Implementation
ParticleContainer::ActiveIterator ParticleContainer::begin()
{
    return { particles.begin(), particles.end(), inactiveParticleMap };
}

ParticleContainer::ActiveIterator ParticleContainer::end()
{
    return { particles.end(), particles.end(), inactiveParticleMap };
}

void ParticleContainer::ActiveIterator::advanceToNextActive()
{
    while (current != end && !current->getActivity()) {
        ++current;
    }
}

ParticleContainer::ActiveIterator::ActiveIterator(
    std::vector<Particle>::iterator start,
    std::vector<Particle>::iterator end,
    std::reference_wrapper<std::map<size_t, size_t>> mapRef)
    : begin(start)
    , current(start)
    , end(end)
    , inactiveMapRef(mapRef)
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

ParticleContainer::ActiveIterator ParticleContainer::ActiveIterator::operator+=(difference_type n)
{
    if (n < 0) {
        this->operator-=(-n);
        return *this;
    }
    for (difference_type i = 0; i < n; ++i)
        ++(*this);
    return *this;
}

ParticleContainer::ActiveIterator ParticleContainer::ActiveIterator::operator-=(difference_type n)
{
    if (n < 0) {
        this->operator+=(-n);
        return *this;
    }
    for (difference_type i = 0; i < n; ++i)
        --(*this);
    return *this;
}

ParticleContainer::ActiveIterator ParticleContainer::ActiveIterator::operator+(difference_type n)
{
    this->operator+=(n);
    return *this;
}

ParticleContainer::ActiveIterator ParticleContainer::ActiveIterator::operator-(difference_type n)
{
    this->operator-=(n);
    return *this;
}

ParticleContainer::ActiveIterator::difference_type ParticleContainer::ActiveIterator::operator-(
    const ActiveIterator& other) const
{
    auto smaller = current;
    auto larger = other.current;
    int factor = -1;
    if (smaller > larger) {
        std::swap(smaller, larger);
        factor = 1;
    }
    auto lower_bound = inactiveMapRef.get().upper_bound(smaller->getID());
    auto upper_bound = inactiveMapRef.get().lower_bound(larger->getID());
    auto delted_dist = std::distance(lower_bound, upper_bound);
    return factor * (std::distance(smaller, larger) - delted_dist);
}

// PairIterator Implementation
//  Returns an iterator to the first pair of particles
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
    ActiveIterator start, ActiveIterator first, ActiveIterator last)
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
