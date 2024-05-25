#pragma once
#include "io/fileReader/FileReader.h"

/**
 * @class XmlReader
 * @brief A class for reading XML files, inheriting from FileReader.
 *
 * XmlReader is a specialized class that inherits from the FileReader class
 * and is designed to read and parse XML files. It overrides the readFile
 * method to implement specific logic for handling XML data.
 */
class XmlReader : public FileReader {
    using FileReader::FileReader;

    /**
     * @brief Reads an XML file and populates the Simulation object.
     *
     * This method overrides the readFile method from the FileReader class.
     * It reads the contents of an XML file and processes the data to
     * initialize or update the Simulation object provided as a parameter.
     *
     * @param sim A reference to the Simulation object to be populated or updated.
     */
    void readFile(Simulation& sim) override;
};
