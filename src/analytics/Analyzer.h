
#pragma once

#include "simulation/LennardJonesDomainSimulation.h"
#include <array>

/**
 * @brief Base class for analyzing simulation data. This is introduced for assignment 5.
 * It is able to calculate density and velocity profiles.
 */
class Analyzer {
public:
    /**
     * @brief Construct a new Analyzer object
     * @param binCounts The number of bins in each dimension. Set a given dimension to 1 to disable binning in that dimension.
     * @param outName The name of the output file.
     */
    Analyzer(std::array<size_t, 3> binCounts, std::string outName);

    /**
     * @brief Analyze the simulation data.
     * @param sim The simulation to analyze.
     */
    virtual void analyze(LennardJonesDomainSimulation& sim);

protected:
    std::array<size_t, 3> binCounts; /**< The number of bins in each dimension. */
    std::string outName; /**< The name of the output file. */
    size_t nBins; /**< The total number of bins. */
    std::array<double, 3> binSize; /**< The width of each bin in each dimension. */
    bool initialized = false; /**< Whether the analyzer has been initialized. */

    /**
     * @brief Write a vector to a CSVFile.
     * @param data The vector to write.
     * @param filename The name of the file to write to.
     */
    static void writeVectorToCSVFile(const std::vector<double>& data, const std::string& filename);

    /**
     * @brief Get the vector index from a bin index.
     * @param binIndex The bin index.
     * @return The vector index.
     */
    [[nodiscard]] size_t getVectorIndexFromBinIndex(const std::array<size_t, 3>& binIndex) const;
};

