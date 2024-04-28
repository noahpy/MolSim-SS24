
# collect all files from tests/
file(GLOB_RECURSE TEST_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# create library from src
file(GLOB_RECURSE SRC_FILES 
    "${PSEMolDyn_GroupF_SOURCE_DIR}/src/*.cpp"
)
add_library(
    src
    STATIC
    ${SRC_FILES}
    )
target_include_directories(
    src
    PUBLIC 
    ${PSEMolDyn_GroupF_SOURCE_DIR}/src
    ${PSEMolDyn_GroupF_SOURCE_DIR}/libs/libxsd
    )
target_link_libraries(src
        PUBLIC
            xerces-c
)

# try to find google test latest version 
find_package(GTest 1.14.0 REQUIRED)

# set up executable
add_executable(tests ${TEST_FILES})
target_link_libraries(tests GTest::gtest GTest::gtest_main src)

# use function from GoogleTest to discover tests
include(GoogleTest)
gtest_discover_tests(tests)
