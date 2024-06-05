
#include "io/fileReader/xmlReader.h"
#include "io/xsd/simulation.h"
#include "models/generators/ParticleGenerator.h"
#include "simulation/lennardJonesSim.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <typeinfo>

void XmlReader::readFile(Simulation& sim)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        spdlog::error("Error: could not open file {}", filename);
        exit(-1);
    }

    try {
        // try to parse file
        std::unique_ptr<simulation_t> sim_input(
            simulation(input_file, xml_schema::flags::dont_validate));

        // read in params
        const auto& params = sim_input->params();
        if (params.delta_t().present())
            sim.delta_t = params.delta_t().get();
        if (params.end_time().present())
            sim.end_time = params.end_time().get();
        if (params.output().present())
            sim.setOutputFile(params.output().get());
        if (params.frequency().present())
            sim.frequency = params.frequency().get();
        if (params.epsilon().present() || params.sigma().present()) {
            // try downcasting if parameters are present
            try {
                LennardJonesSimulation& lj = dynamic_cast<LennardJonesSimulation&>(sim);
                if (params.epsilon().present())
                    lj.setEpsilon(params.epsilon().get());
                if (params.sigma().present())
                    lj.setSigma(params.sigma().get());
            } catch (const std::bad_cast e) {
                spdlog::error("Could not downcast to LennardJonesSimulation: {}", e.what());
                exit(EXIT_FAILURE);
            }
        }

        // read in cluster
        ParticleGenerator generator(sim.container);

        // read cuboids
        const auto& cuboids = sim_input->clusters().cuboid();
        for (auto cuboid : cuboids) {
            generator.registerCluster(std::make_unique<CuboidParticleCluster>(CuboidParticleCluster(
                { cuboid.pos().x(), cuboid.pos().y(), cuboid.pos().z() },
                cuboid.dim().x(),
                cuboid.dim().y(),
                cuboid.dim().z(),
                cuboid.spacing(),
                cuboid.mass(),
                { cuboid.vel().x(), cuboid.vel().y(), cuboid.vel().z() },
                cuboid.brownVel(),
                cuboid.brownDim())));
        }

        // read spheres
        const auto& spheres = sim_input->clusters().sphere();
        for (auto sphere : spheres) {
            // read sphere
            generator.registerCluster(std::make_unique<SphereParticleCluster>(SphereParticleCluster(
                { sphere.center().x(), sphere.center().y(), sphere.center().z() },
                sphere.radius(),
                sphere.sphereDim(),
                sphere.spacing(),
                sphere.mass(),
                { sphere.vel().x(), sphere.vel().y(), sphere.vel().z() },
                sphere.brownVel(),
                sphere.brownDim()
            )));
        }

        generator.generateClusters();

    } catch (const xml_schema::exception& e) {
        spdlog::error("Error when reading: {}", filename);

        std::stringstream ss;
        ss << e;
        std::string line;
        while (std::getline(ss, line, '\n'))
            spdlog::error(line);
        exit(EXIT_FAILURE);
    }
}
