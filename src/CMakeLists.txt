# get all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# remove MolSim.cpp from source files
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/MolSim.cpp")

# create library
add_library(
    src
    STATIC
    ${SRC_FILES}
)

# include headers
target_include_directories(
    src
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PSEMolDyn_GroupF_SOURCE_DIR}/libs/libxsd
)

# include libraries
target_link_libraries(
        src
        PUBLIC
        XercesC::XercesC
        spdlog::spdlog
        OpenMP::OpenMP_CXX
        -static-libstdc++
)

# define MolSim target
add_executable(MolSim MolSim.cpp)
target_link_libraries(MolSim src)
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
            # disable some spam warnings for icpc...
            $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

