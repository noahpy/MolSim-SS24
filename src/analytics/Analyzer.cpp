
#include "Analyzer.h"

#include "utils/ArrayUtils.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <utility>

Analyzer::Analyzer(std::array<size_t, 3> binCounts, std::string outName)
    : binCounts(binCounts)
    , outName(std::move(outName))
    , nBins(binCounts[0] * binCounts[1] * binCounts[2])
    , binSize({ 0, 0, 0 })
    , initialized(false)
{
    spdlog::info(
        "Analyzer initialized with {} bins configured to {}, {}, {}. Writing to: {}",
        nBins,
        binCounts[0],
        binCounts[1],
        binCounts[2],
        outName);

    if (nBins == 0)
        spdlog::warn("A dimension is set to 0 => No bins configured, no output will be written.");
}

void Analyzer::analyze(LennardJonesDomainSimulation& sim)
{
    if (!initialized) {
        binSize[0] = sim.getDomainSize()[0] / (double)binCounts[0];
        binSize[1] = sim.getDomainSize()[1] / (double)binCounts[1];
        binSize[2] = sim.getDomainSize()[2] / (double)binCounts[2];
        initialized = true;
    }

    // Calculate the density profile
    std::vector<double> densityProfile(nBins, 0.0);
    std::vector<double> velocityProfile(nBins, 0.0);
    for (Particle& particle : sim.container.particles) {
        if (!particle.getActivity())
            continue;

        // Calculate the bin index
        std::array<size_t, 3> binIndex = { static_cast<size_t>(particle.getX()[0] / binSize[0]),
                                           static_cast<size_t>(particle.getX()[1] / binSize[1]),
                                           static_cast<size_t>(particle.getX()[2] / binSize[2]) };
        size_t index = getVectorIndexFromBinIndex(binIndex);
        densityProfile[index] += 1.0;
        velocityProfile[getVectorIndexFromBinIndex(binIndex)] +=
            ArrayUtils::L2Norm(particle.getV());
    }
    for (size_t i = 0; i < nBins; i++)
        if (densityProfile[i] > 0.0)
            velocityProfile[i] /= (densityProfile[i] == 0 ? 1 : densityProfile[i]);

    writeVectorToCSVFile(densityProfile, outName + "_density.csv");
    writeVectorToCSVFile(velocityProfile, outName + "_velocity.csv");
}

void Analyzer::writeVectorToCSVFile(const std::vector<double>& data, const std::string& filename)
{
    // Write the data to a CSV file
    if (data.empty())
        return;

    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app);
    if (!outfile.is_open()) {
        spdlog::error("Failed to open file: {}", filename);
        return;
    }
    outfile << data [0];
    for (auto d : data) {
        outfile << "," << d;
    }
    outfile << std::endl;
    outfile.close();
}

size_t Analyzer::getVectorIndexFromBinIndex(const std::array<size_t, 3>& binIndex) const
{
    // Calculate the index of a vector from a bin index i.e. go from 3D to 1D index
    return binIndex[0] + binIndex[1] * binCounts[0] + binIndex[2] * binCounts[0] * binCounts[1];
}
