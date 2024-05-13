find_package(spdlog)

if (NOT spdlog_FOUND)
    message("-- Could not find spdlog, fetching from Github...")
    include(FetchContent)
    FetchContent_Declare(
            spdlog
            GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
            GIT_TAG v1.14.1
    )

    FetchContent_MakeAvailable(spdlog)
endif ()