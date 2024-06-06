/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

/**
 * @brief Class representing a particle in the simulation
 */
class Particle {
private:
    /**
     * @brief Position of the particle
     */
    std::array<double, 3> x;

    /**
     * @brief Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * @brief Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * @brief Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * @brief Mass of this particle
     */
    double m;

    /**
     * @brief Type of the particle.
     * @details Use it for whatever you want (e.g. to separate molecules belonging to different
     * bodies, matters, and so on)
     */
    int type;

public:
    /**
     * @brief Construct a new Particle object of a given type
     * @param type The type of the particle
     * @return Particle object
     */
    explicit Particle(int type = 0);

    /**
     * @brief Construct a new Particle object by copying another Particle object
     * @param other The Particle object to copy
     * @return Particle object
     */
    Particle(const Particle& other);

    /**
     * @brief Construct a new Particle object with given position, velocity, mass and type
     * @param x_arg The position of the particle
     * @param v_arg The velocity of the particle
     * @param m_arg The mass of the particle
     * @param type The type of the particle
     * @return Particle object
     */
    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg,
        std::array<double, 3> v_arg,
        double m_arg,
        int type = 0);

    /**
     * @brief Destroy the Particle object
     * @return void
     */
    virtual ~Particle();

    /**
     * @brief Get the position of the particle
     * @return The position of the particle
     */
    const std::array<double, 3>& getX() const;

    /**
     * @brief Get the velocity of the particle
     * @return The velocity of the particle
     */
    const std::array<double, 3>& getV() const;

    /**
     * @brief Get the force effective on the particle
     * @return The force effective on the particle
     */
    const std::array<double, 3>& getF() const;

    /**
     * @brief Get the force which was effective on the particle
     * @return The force which was effective on the particle
     */
    const std::array<double, 3>& getOldF() const;

    /**
     * @brief Get the mass of the particle
     * @return The mass of the particle
     */
    double getM() const;

    /**
     * @brief Get the type of the particle
     * @return The mass of the particle
     */
    int getType() const;

    /**
     * @brief Set the position of the particle
     * @param x_new The new position of the particle
     * @return void
     */
    void setX(const std::array<double, 3>& x_new);

    /**
     * @brief Set the velocity of the particle
     * @param v_new The new velocity of the particle
     * @return void
     */
    void setV(const std::array<double, 3>& v_new);

    /**
     * @brief Set the force effective on the particle
     * @param f_new The new force effective on the particle
     * @return void
     */
    void setF(const std::array<double, 3>& f_new);

    /**
     * @brief Set the previous force effective on the particle
     * @param f The previous force effective on the particle
     * @return void
     */
    void setOldF(const std::array<double, 3>& f);

    /**
     * @brief Set the mass of the particle
     * @param m_new The new mass of the particle
     * @return void
     */
    void setM(double m_new);

    /**
     * @brief Overload the == operator for the Particle class
     * @param other The other Particle object to compare to
     * @return true if the two Particle objects are value-wise equal, false otherwise
     */
    bool operator==(Particle& other);

    /**
     * @brief Get the string representation of the Particle object
     * @return The string representation of the Particle object
     */
    std::string toString() const;
};

/**
 * @brief Overload the << operator for the Particle class
 * @param stream The output stream
 * @param p The particle object to output
 * @return The stream with the particle object outputted
 */
std::ostream& operator<<(std::ostream& stream, Particle& p);
