
#include "io/fileWriter/writerFactory.h"
#include "io/fileWriter/VTKWriter.h"
#include "io/fileWriter/XYZWriter.h"
#include <spdlog/spdlog.h>

std::unique_ptr<FileWriter> writerFactory(unsigned type)
{
    switch (type) {
    case 0:
        spdlog::info("Initializing VTKWriter...");
        return std::make_unique<outputWriter::VTKWriter>();
    case 1:
        spdlog::info("Initializing XYZWriter...");
        return std::make_unique<outputWriter::XYZWriter>();
    default:
        spdlog::warn("Not a valid writer type {}: Initializing VTK writer.", type);
        return std::make_unique<outputWriter::VTKWriter>();
    }
}
