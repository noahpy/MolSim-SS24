
#pragma once
#include "models/linked_cell/cell/Cell.h"
#include <vector>

class HaloCell : public Cell {
public:
    HaloCell(
        CellIndex myIndex,
        std::vector<unsigned> boundarySides = {},
        std::vector<CellIndex> boundaryNeighbours = {},
        std::vector<CellIndex> haloNeighbours = {});

    // denotes the position / index of this cell
    CellIndex myIndex;

    // denotes which boundries this cell applies to
    std::vector<unsigned> boundarySides;

    // relevant neighbour cells
    std::vector<CellIndex> boundaryNeighbours;
    std::vector<CellIndex> haloNeighbours;
};
