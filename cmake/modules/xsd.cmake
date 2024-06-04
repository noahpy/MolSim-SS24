
# Check if xsdcxx is installed
find_program(XSD_EXECUTABLE NAMES xsdcxx)

# Dont do anything if xsdcxx is not installed
if (XSD_EXECUTABLE)
    file(GLOB_RECURSE ALL_XSD
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/xsd/*.xsd"
    )
    add_custom_target(xsd
        COMMAND ${XSD_EXECUTABLE} cxx-tree --hxx-suffix=.h --cxx-suffix=.cpp --std c++11 --generate-doxygen --generate-serialization ${ALL_XSD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/io/xsd
        COMMENT "Generating xsd code"
        VERBATIM
    )
endif()
