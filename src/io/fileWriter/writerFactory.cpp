
#include "io/fileWriter/writerFactory.h"
#include "io/fileWriter/VTKWriter.h"
#include "io/fileWriter/XYZWriter.h"
#include <spdlog/spdlog.h>

std::unique_ptr<FileWriter> writerFactory(WriterType type, std::string out_name)
{
    switch (type) {
    case WriterType::VTK:
        spdlog::info("Initializing VTKWriter...");
        return std::make_unique<outputWriter::VTKWriter>(out_name);
    case WriterType::XYZ:
        spdlog::info("Initializing XYZWriter...");
        return std::make_unique<outputWriter::XYZWriter>(out_name);
    default:
        spdlog::warn("Not a valid writer type: Initializing VTK writer.");
        return std::make_unique<outputWriter::VTKWriter>(out_name);
    }
}
