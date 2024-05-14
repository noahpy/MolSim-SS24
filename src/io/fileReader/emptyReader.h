
#include "io/fileReader/FileReader.h"

class EmptyFileReader : public FileReader {
    using FileReader::FileReader;
    void readFile(Simulation& sim) override;
};
