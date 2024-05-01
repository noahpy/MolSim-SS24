/*
 *ParticleContainer Class
 */

#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include "Particle.h"
#include <vector>

class ParticleContainer {

public:
    std::vector<Particle> particles;

    ParticleContainer(const std::vector<Particle>& particles);

    // Adds a particle to the container
    void addParticle(const Particle& p);

    std::vector<Particle> getContainer();

    // Returns an iterator to the beginning
    std::vector<Particle>::iterator begin();
    // Returns an iterator to the end
    std::vector<Particle>::iterator end();

    /*
     * Pair Iterator nested class:
     * Iterate from one fixed particle over every possible pair
     * and continue to the next fixed particle.
     */
    class PairIterator {
    private:
        std::vector<Particle>::iterator start, first, second, last;

    public:
        PairIterator(
            std::vector<Particle>::iterator start,
            std::vector<Particle>::iterator first,
            std::vector<Particle>::iterator last);

        // Get last particle
        Particle& getLast();
        // Operators for Iteration traversal
        std::pair<Particle&, Particle&> operator*() const;
        PairIterator& operator++();
        bool operator!=(const PairIterator& other) const;
        bool operator==(const PairIterator& other) const;
    };

    PairIterator beginPairs();
    PairIterator endPairs();
};

#endif // PARTICLECONTAINER_H
