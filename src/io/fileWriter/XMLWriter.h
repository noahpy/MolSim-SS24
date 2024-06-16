
#include "io/fileWriter/FileWriter.h"

namespace outputWriter {
    
class XmlWriter : public FileWriter {
public:
    /**
     * @brief Constructor of the XmlWriter class
     */
    XmlWriter();

    /**
     * @brief This function initializes the XmlWriter class
     * @param out_name the name of the output file
     */
    XmlWriter(std::string out_name) : FileWriter(out_name) {};

    /**
     * @brief Destructor of the XmlWriter class
     */
    virtual ~XmlWriter();

    /**
     * @brief Writes the simulation data to a file for one iteration
     * @param s Simulation object
     */
    void plotParticles(const Simulation& s) override;

};

}
