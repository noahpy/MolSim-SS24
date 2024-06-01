
#include "models/linked_cell/cell/HaloCell.h"
#include "models/linked_cell/cell/CellType.h"

HaloCell::HaloCell(
    CellIndex myIndex,
    std::vector<unsigned> boundarySides,
    std::vector<CellIndex> boundaryNeighbours,
    std::vector<CellIndex> haloNeighbours)
    : myIndex(myIndex)
    , boundarySides(boundarySides)
    , boundaryNeighbours(boundaryNeighbours)
    , haloNeighbours(haloNeighbours)
    , Cell(CellType::Halo)
{
}
