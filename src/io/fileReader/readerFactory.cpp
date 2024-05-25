
#include "io/fileReader/readerFactory.h"
#include "io/fileReader/asciiReader.h"
#include "io/fileReader/clusterReader.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileReader/xmlReader.h"
#include <spdlog/spdlog.h>

std::unique_ptr<FileReader> readerFactory(std::string input_file, unsigned type)
{
    switch (type) {
    case 0:
        spdlog::info("Initializing standard FileReader...");
        return std::make_unique<FileReader>(input_file);
    case 1:
        spdlog::info("Initializing ClusterReader...");
        return std::make_unique<ClusterReader>(input_file);
    case 2:
        spdlog::info("Initializing EmpytFileReader...");
        return std::make_unique<EmptyFileReader>(input_file);
    case 3:
        spdlog::info("Initializing AsciiReader...");
        return std::make_unique<AsciiReader>(input_file);
    case 4:
        spdlog::info("Initializing XmlReader...");
        return std::make_unique<XmlReader>(input_file);
    default:
        spdlog::warn("Not a valid reader type {}: Initializing empty reader.", type);
        return std::make_unique<EmptyFileReader>(input_file);
    }
}
