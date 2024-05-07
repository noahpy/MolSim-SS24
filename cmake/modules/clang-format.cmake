# Check if clang-format is installed
find_program(CLANG_FORMAT NAMES clang-format)

# Dont do anything if clang format is not installed
if (CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    )
    add_custom_target(clangformat
        COMMAND ${CLANG_FORMAT} -i ${ALL_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting file with clang-format"
        VERBATIM
    )
endif()
