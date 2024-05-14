
#include "io/fileReader/FileReader.h"

class ClusterReader : public FileReader {
    using FileReader::FileReader;
    void readFile(Simulation& sim) override;
};
