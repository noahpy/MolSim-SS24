/*
 *ParticleContainer Class
 */

#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include "Particle.h"
#include <unordered_map>
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
     * @brief Get the particles within the container
     * @return The vector of particles
     */
    std::vector<Particle> getContainer() const;

    /**
     * @brief Iterator to the beginning
     * @return An iterator to the beginning
     */
    std::vector<Particle>::iterator begin();
    /**
     * @brief Iterator to the end
     * @return Returns an iterator to the end
     */
    std::vector<Particle>::iterator end();

    /**
     * @brief Forward declaration for iteration over active particles
     */
    class ActiveIterator;

    /**
     * @brief Get the iterator to the first active particle
     * @return The iterator to the first active particle
     */
    ActiveIterator beginActive();

    /**
     * @brief Get the iterator to the last active particle
     * @return The iterator to the last active particle
     */
    ActiveIterator endActive();

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

    /**
     * @brief Iterator class for active particles
     */
    class ActiveIterator {
    private:
       /**
        * @brief Iterator for the ActiveIterator
        */
        std::vector<Particle>::iterator current, end;

       /**
        * @brief Advance to the next active particle
        */
       void advanceToNextActive();

    public:
        /**
         * @brief Construct a new Iterator over active particles
         * @param start The iterator to the first active particle
         * @param end The iterator to the last active particle
         * @return ActiveIterator object
         */
        ActiveIterator(std::vector<Particle>::iterator start, std::vector<Particle>::iterator end);

        /**
         * @brief Override the * operator for range-based for loop
         * @return The reference to the current active particle
         */
        Particle& operator*() const;

        ActiveIterator& operator++();

        bool operator!=(const ActiveIterator& other) const;

        bool operator==(const ActiveIterator& other) const;
    };

    /**
     * @brief Pair Iterator nested class for iterating over all unique pairs of particles
     * @details Iterate from one fixed particle over every possible pair and continue to the next
     * fixed particle.
     */
    class PairIterator {
    private:
        /**
         * @brief The iterators need to iterate over the particle pairs
         */
        std::vector<Particle>::iterator start, first, second, last;

    public:
        /**
         * @brief Construct a new Pair Iterator object
         * @param start The iterator to the first particle
         * @param first The iterator to the first particle
         * @param last The iterator to the last particle
         * @return PairIterator object
         */
        PairIterator(
            std::vector<Particle>::iterator start,
            std::vector<Particle>::iterator first,
            std::vector<Particle>::iterator last);

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
