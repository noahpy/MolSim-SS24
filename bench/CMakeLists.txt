
if (BUILD_BENCH)
    # collect all files from tests/
    file(GLOB_RECURSE BENCH_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )

    # set up executable
    add_executable(benchmarks EXCLUDE_FROM_ALL ${BENCH_FILES})
    target_link_libraries(benchmarks
        PRIVATE
        benchmark::benchmark benchmark::benchmark_main src)

    set_target_properties(benchmark benchmark_main PROPERTIES EXCLUDE_FROM_ALL TRUE)


endif(BUILD_BENCH)
