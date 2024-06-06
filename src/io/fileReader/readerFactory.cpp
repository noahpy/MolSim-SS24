
#include "io/fileReader/readerFactory.h"
#include "io/fileReader/asciiReader.h"
#include "io/fileReader/clusterReader.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileReader/xmlReader.h"
#include <spdlog/spdlog.h>

std::unique_ptr<FileReader> readerFactory(std::string input_file, ReaderType type)
{
    switch (type) {
    case ReaderType::STANDARD:
        spdlog::info("Initializing standard FileReader...");
        return std::make_unique<FileReader>(input_file);
    case ReaderType::CLUSTER:
        spdlog::info("Initializing ClusterReader...");
        return std::make_unique<ClusterReader>(input_file);
    case ReaderType::EMPTY:
        spdlog::info("Initializing EmpytFileReader...");
        return std::make_unique<EmptyFileReader>(input_file);
    case ReaderType::ASCII:
        spdlog::info("Initializing AsciiReader...");
        return std::make_unique<AsciiReader>(input_file);
    case ReaderType::XML:
        spdlog::info("Initializing XmlReader...");
        return std::make_unique<XmlReader>(input_file);
    default:
        spdlog::warn("Not a valid reader type: Initializing empty reader.");
        return std::make_unique<EmptyFileReader>(input_file);
    }
}
