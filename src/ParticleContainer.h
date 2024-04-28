/*
*ParticleContainer Class
*/

#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include "Particle.h"
#include <functional>
#include <vector>


class ParticleContainer {
private:
    //Store Particles in vector
    std::vector<Particle> particles;

public:
    // Adds a particle to the container
    void addParticle(const Particle& p);

    // Returns an iterator to the beginning
    auto begin();
    // Returns an iterator to the end
    auto end();

    /*
     * Pair Iterator nested class:
     * Iterate from one fixed particle over every possible pair
     * and continue to the next fixed particle.
     */
    class PairIterator {
    private:
        std::vector<Particle>::iterator start, first, second, last;

    public:
        PairIterator(std::vector<Particle>::iterator start, std::vector<Particle>:: iterator first, std::vector<Particle>::iterator last);
        // Operators for Iteration traversal
        std::pair<Particle&, Particle&> operator*() const;
        PairIterator& operator++();
        bool operator!=(const PairIterator& other) const;
    };

    PairIterator beginPairs();
    PairIterator endPairs();
};



#endif //PARTICLECONTAINER_H
