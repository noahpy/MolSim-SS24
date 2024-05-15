
#pragma once
#include "io/fileWriter/FileWriter.h"

/**
 * @brief A factory function for creating FileWriter objects.
 * 
 * This function creates and returns a unique pointer to a FileWriter
 * or one of its derived classes based on the file type provided.
 * 
 * @param type An unsigned integer representing the type of file writer to create.
 * @return A unique pointer to a FileWriter object or nullptr if the file type is not supported.
 */
std::unique_ptr<FileWriter> writerFactory(unsigned type);
