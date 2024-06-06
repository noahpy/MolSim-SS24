
#pragma once
#include "io/fileReader/FileReader.h"
#include "utils/Params.h"
#include <memory>
#include <string>

/**
 * @brief A factory function for creating FileReader objects.
 *
 * This function creates and returns a unique pointer to a FileReader
 * or one of its derived classes based on the file type provided.
 *
 * @param input_file The path to the file to be read.
 * @param type An unsigned integer representing the type of file reader to create.
 * @return A unique pointer to a FileReader object or nullptr if the file type is not supported.
 */
std::unique_ptr<FileReader> readerFactory(std::string input_file, ReaderType type);
