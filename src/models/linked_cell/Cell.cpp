
#include "Cell.h"

Cell::Cell() : type(CellType::Inner) {}

Cell::Cell(const CellType type) : type(type) {}

Cell::~Cell() = default;

Cell::Cell(const Cell& other) : type(other.type) {
    for (const auto& particle : other.particles) {
        particles.push_back(std::make_unique<Particle>(*particle));
    }
}

Cell& Cell::operator=(const Cell& other) {
    if (this == &other) return *this;

    type = other.type;
    particles.clear();
    for (const auto& particle : other.particles) {
        particles.push_back(std::make_unique<Particle>(*particle));
    }

    return *this;
}

void Cell::addParticle(std::unique_ptr<Particle> particle) {
    particles.push_back(std::move(particle));
}

void Cell::removeParticle (const Particle* particle) {
    particles.remove_if([particle](const std::unique_ptr<Particle>& p) {return p.get() == particle; });
}

[[nodiscard]] std::list<std::unique_ptr<Particle>>& Cell::getParticles() {
    return particles;
}

CellType Cell::getType() const {
    return type;
}

std::list<std::unique_ptr<Particle>>::iterator Cell::begin() {
    return particles.begin();
}

std::list<std::unique_ptr<Particle>>::iterator Cell::end() {
    return particles.end();
}