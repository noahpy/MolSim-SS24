
#pragma once
#include "io/fileReader/FileReader.h"

/**
 * @brief A class for handling empty files.
 * 
 * This class inherits from FileReader and provides functionality
 * to do nothing about it :)
 */
class EmptyFileReader : public FileReader {
    /**
     * @brief Constructs an EmptyFileReader object with the specified file path.
     * @param filePath The path to the file to be read.
     */
    using FileReader::FileReader;

    /**
     * @brief Reads data from the file into a Simulation object.
     * 
     * This function is overridden to do nothing.
     * @param sim The Simulation object to store the data.
     */
    void readFile(Simulation& sim) override;
};
