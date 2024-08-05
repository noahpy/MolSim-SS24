/*
 *ParticleContainer Class
 */

#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include "Particle.h"
#include <functional>
#include <map>
#include <vector>

/**
 * @brief The ParticleContainer class is a data structure that holds the collection of particles
 */
class ParticleContainer {
public:
    /**
     * @brief Vector that contains all the particles
     */
    std::vector<Particle> particles;

    /**
     * @brief Counter for active particles
     */
    int activeParticleCount;

    /**
     * @brief Map that contains all the inactive particles
     */
    std::map<size_t, size_t> inactiveParticleMap;

    /**
     * @brief Construct a new Particle Container object
     * @param particles The particles to be added to the container
     * @return ParticleContainer object
     */
    ParticleContainer(const std::vector<Particle>& particles);

    /**
     * @brief Construct a new empty Particle Container object
     * @return ParticleContainer object
     */
    ParticleContainer();

    /**
     * @brief Adds a particle to the container
     * @param p The particle to be added
     * @return void
     */
    void addParticle(const Particle& p);

    /**
     * @brief Removes the particles from further calculations
     * @param p The particle to be removed
     * @return void
     */
    void removeParticle(Particle& p);

    /**
     * @brief Get the true index of the particle, ignoring the inactive particles
     * @param id The id of the particle
     * @return The index of the particle
     */
    size_t getIndex(size_t id);

    /**
     * @brief Get the particles within the container
     * @return The vector of particles
     */
    std::vector<Particle> getContainer() const;

    /**
     * @brief Forward declaration for iteration over active particles
     */
    class ActiveIterator;

    /**
     * @brief Get the iterator to the first active particle
     * @return The iterator to the first active particle
     */
    ActiveIterator begin();
    /* std::vector<Particle>::iterator begin(); */

    /**
     * @brief Get the iterator to the last active particle
     * @return The iterator to the last active particle
     */
    ActiveIterator end();
    /* std::vector<Particle>::iterator end(); */

    /**
     * @brief Forward declaration for pair iteration
     */
    class PairIterator;

    /**
     * @brief Get the iterator to the beginning of the pairs
     * @return The iterator to the beginning of the pairs
     */
    PairIterator beginPairs();
    /**
     * @brief Get the iterator to the end of the pairs
     * @return The iterator to the end of the pairs
     */
    PairIterator endPairs();

    ActiveIterator beginActive();

    ActiveIterator endActive();

    /**
     * @brief Iterator class for active particles
     */
    class ActiveIterator {
    private:
        /**
         * @brief Iterator for the ActiveIterator
         */
        std::vector<Particle>::iterator begin, current, end;

        /**
         * @brief Map for the inactive particles
         */
        std::reference_wrapper<std::map<size_t, size_t>> inactiveMapRef;

        /**
         * @brief Advance to the next active particle
         */
        void advanceToNextActive();

    public:
        using difference_type = std::ptrdiff_t;
        /**
         * @brief Construct a new Iterator over active particles
         * @param start The iterator to the first active particle
         * @param end The iterator to the last active particle
         * @return ActiveIterator object
         */
        ActiveIterator(
            std::vector<Particle>::iterator start,
            std::vector<Particle>::iterator begin,
            std::vector<Particle>::iterator end,
            std::reference_wrapper<std::map<size_t, size_t>> inactiveMapRef);

        /**
         * @brief Override the * operator for range-based for loop
         * @return The reference to the current active particle
         */
        Particle& operator*() const;

        ActiveIterator& operator++();

        ActiveIterator& operator--();

        bool operator!=(const ActiveIterator& other) const;

        bool operator==(const ActiveIterator& other) const;

        ActiveIterator operator+=(difference_type n);

        ActiveIterator operator-=(difference_type n);

        ActiveIterator operator+(difference_type n);

        ActiveIterator operator-(difference_type n);

        difference_type operator-(const ActiveIterator& other) const;
    };

    /**
     * @brief Pair Iterator nested class for iterating over all unique pairs of particles
     * @details Iterate from one fixed particle over every possible pair and continue to the next
     * fixed particle.
     */
    class PairIterator {
    public:
        /**
         * @brief The iterators need to iterate over the particle pairs
         */
        ActiveIterator start, first, second, last;

        /**
         * @brief Construct a new Pair Iterator object
         * @param start The iterator to the first particle
         * @param first The iterator to the first particle
         * @param last The iterator to the last particle
         * @return PairIterator object
         */
        PairIterator(ActiveIterator start, ActiveIterator first, ActiveIterator last);

        /**
         * @brief Get the last particle
         * @return The last particle
         */
        Particle& getLast();
        /**
         * @brief Override the * operator for range-based for loop
         * @return A the reference to the current pair of particles
         */
        std::pair<Particle&, Particle&> operator*() const;
        /**
         * @brief Override the ++ operator for range-based for loop
         * @return The next pair of particles
         */
        PairIterator& operator++();
        /**
         * @brief Override the != operator for range-based for loop
         * @param other The other PairIterator object to be compared to
         * @return Whether the two PairIterator objects are not equal
         */
        bool operator!=(const PairIterator& other) const;
        /**
         * @brief Override the == operator for range-based for loop
         * @param other The other PairIterator object to be compared to
         * @return Return yes if the two PairIterator objects are equal, false otherwise
         */
        bool operator==(const PairIterator& other) const;
    };
};

#endif // PARTICLECONTAINER_H
