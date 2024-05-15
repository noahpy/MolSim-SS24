

#pragma once
#include "io/fileReader/FileReader.h"

class AsciiReader: public FileReader {
    using FileReader::FileReader;

    void readFile(Simulation& sim) override;
};
