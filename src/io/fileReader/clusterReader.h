
#pragma once
#include "io/fileReader/FileReader.h"

/**
 * @brief A class for reading cluster data from a file.
 *
 * This class inherits from FileReader and provides functionality
 * to read cluster data from a file into a Simulation object.
 */
class ClusterReader : public FileReader {
    /**
     * @brief Constructs a ClusterReader object with the specified file path.
     * @param filePath The path to the file to be read.
     */
    using FileReader::FileReader;

    /**
     * @brief Reads cluster data from the file into a Simulation object.
     * @param sim The Simulation object to store the cluster data.
     */
    void readFile(Simulation& sim) override;
};
