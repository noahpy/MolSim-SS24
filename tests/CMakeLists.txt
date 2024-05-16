
enable_testing()

# collect all files from tests/
file(GLOB_RECURSE TEST_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# set up executable
add_executable(tests ${TEST_FILES})
target_link_libraries(tests 
    PRIVATE
    GTest::gtest_main src)

# use function from GoogleTest to discover tests
include(GoogleTest)
gtest_discover_tests(tests)
