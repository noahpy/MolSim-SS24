

option(BUILD_BENCH "Build benchmarks" ON)

if (BUILD_BENCH)
    # Disable the Google Benchmark requirement on Google Test
    set(BENCHMARK_ENABLE_TESTING NO)
    find_package(benchmark QUIET)
    if(NOT BENCHMARK_FOUND)
        message("-- INFO: Google Benchmark not found, fetching...")
        include(FetchContent)
        FetchContent_Declare(
            googlebenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG main
        )
        FetchContent_MakeAvailable(googlebenchmark)
    endif(NOT BENCHMARK_FOUND)

endif(BUILD_BENCH)
