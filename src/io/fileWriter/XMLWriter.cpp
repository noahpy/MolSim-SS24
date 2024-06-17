
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/Position.h"
#include <fstream>
#include <io/fileWriter/XMLWriter.h>
#include <io/xsd/simulation.h>
#include <iomanip>
#include <spdlog/spdlog.h>

namespace outputWriter {
void XmlWriter::plotParticles(const Simulation& s)
{
    // Set params
    std::unique_ptr<params_t> params = std::make_unique<params_t>();
    params->start_time(s.time);
    params->delta_t(s.delta_t);
    params->end_time(s.end_time);
    params->output(s.getOutputFile());
    params->frequency(s.frequency);

    // try to downcast and set params
    // try LJSim
    try {
        auto& ljs = dynamic_cast<const LennardJonesSimulation&>(s);
        params->epsilon(ljs.getEpsilon());
        params->sigma(ljs.getSigma());
    } catch (const std::bad_cast& e) {
        spdlog::debug("When writing to XML, not a LennardJonesSimulation");
    }
    // try LinkedLJSim
    try {
        auto& ljs = dynamic_cast<const LinkedLennardJonesSimulation&>(s);
        auto domainSize = ljs.getDomainSize();
        params->domainSize({ domainSize.at(0), domainSize.at(1), domainSize.at(2) });
        auto domainOrigin = ljs.getDomainOrigin();
        params->domainOrigin({ domainOrigin.at(0), domainOrigin.at(1), domainOrigin.at(2) });
        params->cutoff(ljs.getCutoff());
        params->updateFreq(ljs.getUpdateFrequency());
    } catch (const std::bad_cast& e) {
        spdlog::debug("When writing to XML, not a LinkedLennardJonesSimulation");
    }
    // try LJDomainSim
    try {
        auto& ljs = dynamic_cast<const LennardJonesDomainSimulation&>(s);
        std::unique_ptr<boundary_t> boundaries = std::make_unique<boundary_t>();
        if (ljs.bcHandler.dimensionality == 2) {
            std::string left_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::LEFT));
            boundaries->bound_four().push_back(left_bound);

            std::string right_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::RIGHT));
            boundaries->bound_four().push_back(right_bound);

            std::string top_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::TOP));
            boundaries->bound_four().push_back(top_bound);

            std::string bottom_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::BOTTOM));
            boundaries->bound_four().push_back(bottom_bound);
        } else {
            std::string left_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::LEFT));
            boundaries->bound_six().push_back(left_bound);

            std::string right_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::RIGHT));
            boundaries->bound_six().push_back(right_bound);

            std::string top_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::TOP));
            boundaries->bound_six().push_back(top_bound);

            std::string bottom_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::BOTTOM));
            boundaries->bound_six().push_back(bottom_bound);

            std::string front_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::FRONT));
            boundaries->bound_six().push_back(front_bound);

            std::string back_bound =
                getBoundaryString(ljs.bcHandler.boundaryConfig.boundaryMap.at(Position::BACK));
            boundaries->bound_six().push_back(back_bound);
        }
        params->boundaries(std::move(boundaries));
    } catch (const std::bad_cast& e) {
        spdlog::debug("When writing to XML, not a LennardJonesDomainSimulation");
    }

    // TODO: Write thermostat
    // TODO: Write gravity

    // Initialize particle data
    DecimalArray_t points = DecimalArray_t(3);
    DecimalArray_t vels = DecimalArray_t(3);
    DecimalArray_t forces = DecimalArray_t(3);
    DecimalArray_t oldForces = DecimalArray_t(3);
    DecimalArray_t mass = DecimalArray_t(1);
    DecimalArray_t type = DecimalArray_t(1);

    // Fill particle data
    for (auto& p : s.container) {
        points.push_back(p.getX().at(0));
        points.push_back(p.getX().at(1));
        points.push_back(p.getX().at(2));
        vels.push_back(p.getV().at(0));
        vels.push_back(p.getV().at(1));
        vels.push_back(p.getV().at(2));
        forces.push_back(p.getF().at(0));
        forces.push_back(p.getF().at(1));
        forces.push_back(p.getF().at(2));
        oldForces.push_back(p.getOldF().at(0));
        oldForces.push_back(p.getOldF().at(1));
        oldForces.push_back(p.getOldF().at(2));
        mass.push_back(p.getM());
        type.push_back(p.getType());
    }

    // Set particle data
    ParticleData_t parts = ParticleData_t(points, vels, forces, oldForces, mass, type);

    // Initialize final simulation object
    std::unique_ptr<clusters_t> clusters = std::make_unique<clusters_t>();
    std::unique_ptr<ParticleTypes_t> ptypes = std::make_unique<ParticleTypes_t>();
    std::unique_ptr<simulation_t> sim =
        std::make_unique<simulation_t>(std::move(params), std::move(clusters));
    sim->ptypes(std::move(ptypes));
    sim->particles(parts);

    // Initialize output file
    std::stringstream strstr;
    strstr << this->out_name << "_" << std::setfill('0') << std::setw(4) << s.iteration << ".xml";
    std::ofstream file(strstr.str().c_str());

    // Write to file
    simulation(file, *sim.get());
}

XmlWriter::XmlWriter() = default;
XmlWriter::~XmlWriter() = default;

}
