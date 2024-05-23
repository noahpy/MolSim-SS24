
#include "Cell.h"

Cell::Cell(const CellType type) : type(type) {}

Cell::~Cell() = default;

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