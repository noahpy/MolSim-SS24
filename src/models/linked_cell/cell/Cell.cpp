
#include "Cell.h"
#include <spdlog/spdlog.h>

Cell::Cell(const CellIndex index)
    : type(CellType::Inner)
    , neighborCounter(0)
    , myIndex(index)
    , boundaryNeighbours()
    , haloNeighbours()
    , innerNeighbours()
{
}

Cell::Cell(const CellType type, const CellIndex index)
    : type(type)
    , neighborCounter(0)
    , myIndex(index)
    , boundaryNeighbours()
    , haloNeighbours()
    , innerNeighbours()

{
}

Cell::Cell(const Cell& other)
    : type(other.type)
    , neighborCounter(other.neighborCounter)
    , myIndex(other.myIndex)
    , boundaryNeighbours(other.boundaryNeighbours)
    , haloNeighbours(other.haloNeighbours)
    , innerNeighbours(other.innerNeighbours)
    , mutex()
{
}

Cell::~Cell() = default;

void Cell::addParticle(Particle& particle)
{
    std::reference_wrapper<Particle> particleRef(particle);
    particles.emplace_back(particleRef);
}

void Cell::removeParticle(Particle& particle)
{
    /*
     * It is necessary to use the pointer to be able to pass it to the lambda function
     * Intern the pointer is necessary because if we pass the particle object, it will be copied
     * Thus changing its memory address and the comparison will not work
     *
     * Before it was done like so: [particle](const std::reference_wrapper<particle>& p) { return
     * &p.get() == &particle; }
     */
    const Particle* particlePtr = &particle;
    particles.remove_if([particlePtr](const std::reference_wrapper<Particle>& p) {
        return &p.get() == particlePtr;
    });
}

void Cell::clearParticles()
{
    particles.clear();
}

CellType Cell::getType() const
{
    return type;
}

ParticleRefList::iterator Cell::begin()
{
    return particles.begin();
}

ParticleRefList::iterator Cell::end()
{
    return particles.end();
}

void Cell::setCounter(int count)
{
    neighborCounter = count;
}

Cell::PairListIterator::PairListIterator(ParticleRefList& particles, bool end)
    : particles(particles)
    , firstIt(particles.begin())
    , secondIt(particles.begin())
{
    if (!end && particles.size() > 1) {
        if (secondIt != particles.end()) {
            ++secondIt;
        }
    } else {
        firstIt = particles.end();
        secondIt = particles.end();
    }
}

void Cell::checkVisitedReset() 
{
    if (!visited) {
        visited = true;
        // reset particles
        for (auto& p : particles) {
            p.get().resetF();
        }
    }
}

void Cell::checkVisitedResetSafe(){
    std::lock_guard<std::mutex> lock(mutex);
    checkVisitedReset();
}

bool Cell::checkNeighbourCounter() 
{
    if (neighborCounter > 0){
        --neighborCounter;
        if(!neighborCounter){
            visited = false;
        }
        return true;
    }
    return false;
}

bool Cell::visit() THREAD_SAFE
{
    std::lock_guard<std::mutex> lock(mutex);
    if (checkNeighbourCounter()){
        return false;
    }
    checkVisitedReset();
    return true;
}

std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>> Cell::
    PairListIterator::operator*() const
{
    return { (*firstIt).get(), (*secondIt).get() };
}

Cell::PairListIterator& Cell::PairListIterator::operator++()
{
    if (firstIt == particles.end()) {
        return *this;
    }
    ++secondIt;
    if (secondIt == particles.end()) {
        ++firstIt;
        if (firstIt != particles.end()) {
            secondIt = std::next(firstIt);
            if (secondIt == particles.end()) {
                firstIt = particles.end();
            }
        }
    }

    return *this;
}

bool Cell::PairListIterator::operator!=(const PairListIterator& other) const
{
    return firstIt != other.firstIt || secondIt != other.secondIt;
}

Cell::PairListIterator Cell::beginPairs()
{
    return PairListIterator(particles, false);
}

Cell::PairListIterator Cell::endPairs()
{
    return PairListIterator(particles, true);
}
