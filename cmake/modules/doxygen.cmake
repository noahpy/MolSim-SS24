# Check if Doxygen is installed
find_package(Doxygen)

# Dont do anything if Doxygen is not installed
if (DOXYGEN_FOUND)
    # Create an option to turn on/off the generation of the documentation
    option(GENERATE_DOC "Generate documentation with Doxygen" ON)
    if (GENERATE_DOC)
        # set input Doxyfile
        set(DOXYGEN_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

        # add a target to generate the documentation
        # i.e. create documentation by make doc_doxygen
        add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} # Folder to use as root to discover the input files
                COMMENT "Generating documentation with Doxygen"
                VERBATIM)
    endif (GENERATE_DOC)
endif (DOXYGEN_FOUND)