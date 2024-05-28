
#pragma once

#include "CellType.h"
#include "models/Particle.h"
#include <list>
#include <memory>

typedef std::array<size_t, 3> CellIndex;

// Class representing a cell in the grid
class Cell {
public:
    // Default constructor
    Cell();

    // Constructor with cell type
    explicit Cell(CellType type);

    // Define copy constructor, had to do this because of some error
    Cell(const Cell& other);

    // Destructor
    ~Cell();

    // Define copy assignment operator, had to do this because of some error
    Cell& operator=(const Cell& other);

    // Method to add a particle to the cell
    void addParticle(std::unique_ptr<Particle> particle);

    // Method to remove a particle from the cell
    void removeParticle(const Particle* particle);

    // Method to get all particles in the cell
    [[nodiscard]] std::list<std::unique_ptr<Particle>>& getParticles();

    // Method to get the type of the cell
    [[nodiscard]] CellType getType() const;

    // Method to get an iterator to the beginning of the particles list
    std::list<std::unique_ptr<Particle>>::iterator begin();

    // Method to get an iterator to the end of the particles list
    std::list<std::unique_ptr<Particle>>::iterator end();

    // Getter and setter for neighborCounter
    [[nodiscard]] int getCounter() const;
    void setCounter(int);

private:
    // Type of the cell
    CellType type;

    // Counter for processed neighbors
    int neighborCounter;

    // List to store pointers to particles in the cell
    std::list<std::unique_ptr<Particle>> particles;
};
