
# try to find google test latest version 
find_package(GTest 1.14.0)

if (NOT GTEST_FOUND)
    message("-- Could not find googletest, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
    )

    FetchContent_MakeAvailable(googletest)
endif()
