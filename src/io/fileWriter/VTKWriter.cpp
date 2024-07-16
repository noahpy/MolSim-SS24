/*
 * VTKWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "io/fileWriter/VTKWriter.h"

#include <cstdlib>
#include <fstream>
#include <io/xsd/simulation.h>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <string>

namespace outputWriter {

VTKWriter::VTKWriter() = default;

VTKWriter::~VTKWriter() = default;

void VTKWriter::plotParticles(const Simulation& s)
{
    initializeOutput(s.container.activeParticleCount);
#pragma omp parallel for
    for (auto& p : s.container) {
        plotParticle(p, s.container.getIndex(p.getID()));
    }

    writeFile(this->out_name, s.iteration);
}

void VTKWriter::initializeOutput(int numParticles)
{
    vtkFile = new VTKFile_t("UnstructuredGrid");

    // per point, we add type, position, velocity and force
    PointData pointData;
    DataArray_t mass(type::Float32, "mass", 1);
    DataArray_t velocity(type::Float32, "velocity", 3);
    DataArray_t forces(type::Float32, "force", 3);
    DataArray_t type(type::Int32, "type", 1);

    mass.resize(numParticles);
    velocity.resize(numParticles * 3);
    forces.resize(numParticles * 3);
    type.resize(numParticles);

    pointData.DataArray().push_back(mass);
    pointData.DataArray().push_back(velocity);
    pointData.DataArray().push_back(forces);
    pointData.DataArray().push_back(type);

    CellData cellData; // we don't have cell data => leave it empty

    // 3 coordinates
    Points points;
    DataArray_t pointCoordinates(type::Float32, "points", 3);
    pointCoordinates.resize(numParticles * 3);
    points.DataArray().push_back(pointCoordinates);

    Cells cells; // we don't have cells, => leave it empty
    // for some reasons, we have to add a dummy entry for paraview
    DataArray_t cells_data(type::Float32, "types", 0);
    cells.DataArray().push_back(cells_data);

    PieceUnstructuredGrid_t piece(pointData, cellData, points, cells, numParticles, 0);
    UnstructuredGrid_t unstructuredGrid(piece);
    vtkFile->UnstructuredGrid(unstructuredGrid);
}

void VTKWriter::writeFile(const std::string& filename, int iteration)
{
    std::stringstream strstr;
    strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".vtu";

    std::ofstream file(strstr.str().c_str());
    VTKFile(file, *vtkFile);
    delete vtkFile;
}

void VTKWriter::plotParticle(Particle& p, size_t index)
{
    if (vtkFile->UnstructuredGrid().present()) {
        spdlog::trace("UnstructuredGrid is present");
    } else {
        spdlog::error("No UnstructuredGrid present", 1);
    }

    PointData::DataArray_sequence& pointDataSequence =
        vtkFile->UnstructuredGrid()->Piece().PointData().DataArray();
    PointData::DataArray_iterator dataIterator = pointDataSequence.begin();

    dataIterator->at(index) = p.getM();

    dataIterator++;
    dataIterator->at(3 * index) = p.getV()[0];
    dataIterator->at(3 * index + 1) = p.getV()[1];
    dataIterator->at(3 * index + 2) = p.getV()[2];

    dataIterator++;

    dataIterator->at(3 * index) = p.getF()[0];
    dataIterator->at(3 * index + 1) = p.getF()[1];
    dataIterator->at(3 * index + 2) = p.getF()[2];

    dataIterator++;
    dataIterator->at(index) = p.getType();

    Points::DataArray_sequence& pointsSequence =
        vtkFile->UnstructuredGrid()->Piece().Points().DataArray();
    Points::DataArray_iterator pointsIterator = pointsSequence.begin();
    pointsIterator->at(3 * index) = p.getX()[0];
    pointsIterator->at(3 * index + 1) = p.getX()[1];
    pointsIterator->at(3 * index + 2) = p.getX()[2];
}

} // namespace outputWriter
