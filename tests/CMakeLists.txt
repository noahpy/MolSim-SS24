
# collect all files from tests/
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)


# try to find google test latest version 
find_package(GTest 1.14.0 REQUIRED)

# set up executable
add_executable(tests ${MY_SRC})
target_link_libraries(tests GTest::gtest GTest::gtest_main)
target_include_directories(tests 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src")

# use function from GoogleTest to discover tests
include(GoogleTest)
gtest_discover_tests(tests)
