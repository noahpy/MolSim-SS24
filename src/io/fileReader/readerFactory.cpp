
#include "io/fileReader/readerFactory.h"
#include "io/fileReader/clusterReader.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileReader/asciiReader.h"
#include <spdlog/spdlog.h>

std::unique_ptr<FileReader> readerFactory(std::string input_file, unsigned type)
{
    switch (type) {
    case 0:
        return std::make_unique<FileReader>(input_file);
    case 1:
        return std::make_unique<ClusterReader>(input_file);
    case 2:
        return std::make_unique<EmptyFileReader>(input_file);
    case 3:
        return std::make_unique<AsciiReader>(input_file);
    default:
        spdlog::warn("Not a valid reader type {}: Initializing empty reader.", type);
        return std::make_unique<EmptyFileReader>(input_file);
    }
}
