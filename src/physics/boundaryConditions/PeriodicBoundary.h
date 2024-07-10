
#pragma once
#include "physics/boundaryConditions/BoundaryCondition.h"
#include "physics/boundaryConditions/BoundaryConfig.h"

// forward declare
class CellGrid;

/**
 * @brief Type to contain the shifts needed to move particles to their periodic partner and the
 * partner cell index
 * @details The first entry is the positional shift, while the second entry is the cell index shift
 */
typedef std::pair<std::array<double, 3>, std::array<int, 3>> PeriodicBoundShifts;
/**
 * @brief Type to contain the shifts of particles that are effected by more than one periodic
 * boundary
 * @details The std::vector<Position> is the list of sides that are effecting on the particle, while
 * the std::vector<PeriodicBoundShifts> are the shifts to be applied
 */
typedef std::map<std::vector<Position>, std::vector<PeriodicBoundShifts>>
    MultiDimPeriodicBoundShiftsMap;

class PeriodicBoundary : public BoundaryCondition {
public:
    /**
     * @brief The constructor of the PeriodicBoundary. The constructor gets the config passed, as
     * the boundary behaves differently if the neighboring boundaries are periodic too
     * @param position The position of the boundary
     * @param boundaryConfig The configuration of the boundaries of the simulation
     * @param cellGrid The cell grid to perform the periodic boundaries in
     * @return The Periodic boundary
     */
    PeriodicBoundary(
        Position position, const BoundaryConfig& boundaryConfig, const CellGrid& cellGrid);

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This will introduce the halo particles for all boundary particles
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * Removes all inserted halo particles used for the boundary particles and moves any particles,
     * that have crossed the border
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation) override;

    /**
     * @brief Getter for the translationMap
     * @return The translation map
     */
    MultiDimPeriodicBoundShiftsMap getTranslationMap() const { return translationMap; }

private:
    std::vector<std::unique_ptr<Particle>>
        insertedParticles; /**< The particles that were inserted. These will be recycled */
    size_t insertionIndex; /**< The index of the next particle to recycle */

    bool is2D; /**< Whether this is a 2D bound or 3D */
    PeriodicBoundShifts innerTranslation; /**< The translations to apply to cells that are only
                                             affected by one boundary */
    MultiDimPeriodicBoundShiftsMap translationMap; /**< The translations to apply to cells that are
                                                      only affected by two boundaries */

    /**
     * @brief get the appropriate shifts for the position of the boundary
     * @param grid The grid that the shifts will be performed in
     * @return The shifts
     */
    PeriodicBoundShifts getPeriodicShift(const CellGrid& grid)
    {
        return getPeriodicShiftFromPosition(position, grid);
    }
    /**
     * @brief get the appropriate shifts for any position
     * @param pos The position to get the shifts for
     * @param grid The grid that the shifts will be performed in
     * @return The shifts
     */
    static PeriodicBoundShifts getPeriodicShiftFromPosition(Position pos, const CellGrid& grid);

    /**
     * @brief Helper function to actually generate the translations needed
     * @param key The key to insert into in the map
     * @param boundarySides The sides to generate the translations for
     * @param cellGrid The grid to perform the translations in
     */
    void fillTranslationMap(
        const std::vector<Position>& key,
        const std::vector<Position>& boundarySides,
        const CellGrid& cellGrid);
};
