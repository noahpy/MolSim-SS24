
#include "models/linked_cell/BoundaryCell.h"
#include "models/linked_cell/Cell.h"
#include "models/linked_cell/CellType.h"

BoundaryCell::BoundaryCell(
    CellIndex myIndex,
    std::vector<unsigned> boundarySides,
    std::vector<CellIndex> boundaryNeighbours,
    std::vector<CellIndex> haloNeighbours,
    std::vector<CellIndex> innerNeighbours)
    : myIndex(myIndex)
    , boundarySides(boundarySides)
    , boundaryNeighbours(boundaryNeighbours)
    , haloNeighbours(haloNeighbours)
    , innerNeighbours(innerNeighbours)
    , Cell(CellType::Boundary)
{
}
