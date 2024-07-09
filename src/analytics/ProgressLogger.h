#pragma once

#include <chrono>
#include <cmath>
#include <spdlog/spdlog.h>
/**
 * @class ProgressLogger
 * @brief A class for logging progress during a simulation.
 */
class ProgressLogger {
public:
    /**
     * @brief Constructs a ProgressLogger object with the specified start time, end time, and delta
     * time.
     * @param startTime The start time of the simulation.
     * @param endTime The end time of the simulation.
     * @param deltaT The time steps of the simulation.
     */
    ProgressLogger(double startTime, double endTime, double deltaT)
        : totalIterations(static_cast<unsigned>(std::ceil((endTime - startTime) / deltaT)))
        , startTime(std::chrono::steady_clock::now())
        , nextLoggingIteration(totalIterations / 100)
    {
    }

    /**
     * @brief Logs the progress of the simulation. This will only write a log if a new percentage is
     * reached.
     * @param currentIteration The current iteration of the simulation.
     */
    inline void logProgress(unsigned currentIteration)
    {
        // only log, if new percentage is reached -> 100 updates
        if (currentIteration < nextLoggingIteration)
            return;

        auto currentTime = std::chrono::steady_clock::now();
        long elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        double progress = static_cast<double>(currentIteration) / totalIterations;
        long estimatedTimeLeft =
            static_cast<long>((static_cast<double>(elapsedTime) / progress)) - elapsedTime;

        spdlog::info(
            "Progress: {:.2f}%, Estimated time left (DD:HH:MM:SS): {:02}:{:02}:{:02}:{:02}",
            progress * 100,
            estimatedTimeLeft / 86400000l,
            (estimatedTimeLeft % 86400000l) / 3600000l,
            (estimatedTimeLeft % 3600000l) / 60000l,
            (estimatedTimeLeft % 60000l) / 1000l);

        nextLoggingIteration =
            static_cast<unsigned>(static_cast<double>(totalIterations) * (progress + 0.01));
    };

private:
    unsigned totalIterations; /**< The total number of iterations in the simulation. */
    std::chrono::steady_clock::time_point startTime; /**< The start time of the simulation. */
    unsigned nextLoggingIteration; /**< Next iteration a log will be added. */
};
