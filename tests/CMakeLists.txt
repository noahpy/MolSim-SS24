
# collect all cpp files
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# try to find package
find_package(GTest REQUIRED)

# set up executable
add_executable(tests ${MY_SRC})
target_link_libraries(tests GTest::gtest GTest::gtest_main)

# use function from GoogleTest to discover tests
include(GoogleTest)
gtest_discover_tests(tests)
