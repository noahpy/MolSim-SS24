
#include "CellGrid.h"
#include "models/linked_cell/cell/Cell.h"

/* ########### BoundaryIterator Implementation ########### */
CellGrid::BoundaryIterator::BoundaryIterator(std::vector<CellIndex>& boundaries, bool end)
    : boundaries(boundaries)
{
    if (end) {
        index = boundaries.size();
        return;
    }
    index = 0;
}

CellIndex CellGrid::BoundaryIterator::operator*() const
{
    if (index < boundaries.size())
        return boundaries.at(index);
    return { 0, 0, 0 };
}

CellGrid::BoundaryIterator& CellGrid::BoundaryIterator::operator++()
{
    if (index < boundaries.size())
        ++index;
    return *this;
}

bool CellGrid::BoundaryIterator::operator!=(const BoundaryIterator& other) const
{
    return this->index != other.index;
}

/* ########### HaloIterator Implementation ########### */
CellGrid::HaloIterator::HaloIterator(std::vector<CellIndex>& halos, bool end)
    : halos(halos)
{
    if (end) {
        index = halos.size();
        return;
    }
    index = 0;
}

CellIndex CellGrid::HaloIterator::operator*() const
{
    if (index < halos.size())
        return halos.at(index);
    return { 0, 0, 0 };
}

CellGrid::HaloIterator& CellGrid::HaloIterator::operator++()
{
    if (index < halos.size())
        ++index;
    return *this;
}

bool CellGrid::HaloIterator::operator!=(const HaloIterator& other) const
{
    return this->index != other.index;
}

/* ########### PairIterator Implementation ########### */
CellGrid::PairIterator::PairIterator(ParticleRefList& particles, bool end)
    : particles(particles)
    , firstIt(particles.begin())
    , secondIt(particles.begin())
{
    if (!end) {
        if (secondIt != particles.end()) {
            ++secondIt;
        }
    } else {
        firstIt = particles.end();
        secondIt = particles.end();
    }
}

std::pair<Particle*, Particle*> CellGrid::PairIterator::operator*() const
{
    return { &(*firstIt).get(), &(*secondIt).get() };
}

CellGrid::PairIterator& CellGrid::PairIterator::operator++()
{
    advance();
    return *this;
}

bool CellGrid::PairIterator::operator!=(const PairIterator& other) const
{
    return firstIt != other.firstIt || secondIt != other.secondIt;
}

void CellGrid::PairIterator::advance()
{
    if (firstIt == particles.end()) {
        return;
    }

    if (secondIt == particles.end()) {
        ++firstIt;
        if (firstIt != particles.end()) {
            secondIt = std::next(firstIt);
        }
    } else
        ++secondIt;
}

CellGrid::PairIterator CellGrid::PairIterator::beginPairs(ParticleRefList& particles)
{
    return PairIterator(particles, false);
}

CellGrid::PairIterator CellGrid::PairIterator::endPairs(ParticleRefList& particles)
{
    return PairIterator(particles, true);
}
