# Check if Doxygen is installed
find_package(Doxygen)

# Dont do anything if Doxygen is not installed
if (DOXYGEN_FOUND)
    # Create an option to turn on/off the generation of the documentation
    option(GENERATE_DOC "Generate documentation with Doxygen" ON)
    if (GENERATE_DOC)
        # set input Doxyfile and output directory for the documentation
        set(DOXYGEN_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
        set(DOCUMENTATION_OUT ${CMAKE_CURRENT_BINARY_DIR}/documentation)

        # request to configure the file
        # @ONLY will only replace variables of the form @VAR@
        configure_file(${DOXYGEN_CONFIG} ${DOCUMENTATION_OUT} @ONLY)

        # add a target to generate the documentation
        # i.e. create documentation by make doc_doxygen
        add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOCUMENTATION_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating documentation with Doxygen"
                VERBATIM)
    endif (GENERATE_DOC)
endif (DOXYGEN_FOUND)