
#pragma once

enum class CellType { Inner, Boundary, Halo };

inline std::string getCellTypeString(CellType cellType)
{
    switch (cellType) {
    case CellType::Inner:
        return "Inner";
    case CellType::Boundary:
        return "Boundary";
    case CellType::Halo:
        return "Halo";
    }
}
