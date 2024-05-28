
#pragma once
#include "models/linked_cell/Cell.h"
#include <vector>


class BoundaryCell : public Cell {
public:
    BoundaryCell(
        CellIndex myIndex,
        std::vector<unsigned> boundarySides = {},
        std::vector<CellIndex> boundaryNeighbours = {},
        std::vector<CellIndex> haloNeighbours = {},
        std::vector<CellIndex> innerNeighbours = {});

    // denotes the position / index of this cell
    CellIndex myIndex;

    // denotes which boundries this cell applies to
    std::vector<unsigned> boundarySides;

    // relevant neighbours
    std::vector<CellIndex> boundaryNeighbours;
    std::vector<CellIndex> haloNeighbours;
    std::vector<CellIndex> innerNeighbours;
};
