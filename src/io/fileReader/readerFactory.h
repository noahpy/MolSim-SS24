
#include "io/fileReader/FileReader.h"
#include <memory>
#include <string>

std::unique_ptr<FileReader> readerFactory(std::string input_file, unsigned type);
