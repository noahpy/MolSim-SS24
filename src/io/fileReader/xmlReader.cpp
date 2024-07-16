
#include "io/fileReader/xmlReader.h"
#include "io/xsd/simulation.h"
#include "models/generators/ParticleGenerator.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>

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

        // read in cluster
        ParticleGenerator generator(sim.container);

        // read cuboids
        const auto& cuboids = sim_input->clusters().cuboid();
        for (auto cuboid : cuboids) {
            unsigned ptype = 0;
            if (cuboid.ptype().present())
                ptype = cuboid.ptype().get();
            // read cuboid
            generator.registerCluster(std::make_unique<CuboidParticleCluster>(CuboidParticleCluster(
                { cuboid.pos().x(), cuboid.pos().y(), cuboid.pos().z() },
                cuboid.dim().x(),
                cuboid.dim().y(),
                cuboid.dim().z(),
                cuboid.spacing(),
                cuboid.mass(),
                { cuboid.vel().x(), cuboid.vel().y(), cuboid.vel().z() },
                cuboid.brownVel(),
                cuboid.brownDim(),
                sim.stationaryParticleTypes,
                ptype)));
        }

        // read spheres
        const auto& spheres = sim_input->clusters().sphere();
        for (auto sphere : spheres) {
            unsigned ptype = 0;
            if (sphere.ptype().present())
                ptype = sphere.ptype().get();
            // read sphere
            generator.registerCluster(std::make_unique<SphereParticleCluster>(SphereParticleCluster(
                { sphere.center().x(), sphere.center().y(), sphere.center().z() },
                sphere.radius(),
                sphere.sphereDim(),
                sphere.spacing(),
                sphere.mass(),
                { sphere.vel().x(), sphere.vel().y(), sphere.vel().z() },
                sphere.brownVel(),
                sphere.brownDim(),
                sim.stationaryParticleTypes,
                ptype)));
        }

        generator.generateClusters();

        if (sim_input->particles().present()) {
            const auto& particles = sim_input->particles().get();
            // check if all data arrays have the corresponding size
            if (!(particles.MassData().size() == particles.TypeData().size() &&
                  particles.PointData().size() / 3 == particles.VelData().size() / 3 &&
                  particles.ForceData().size() / 3 == particles.OldForceData().size() / 3 &&
                  particles.MassData().size() == particles.PointData().size() / 3) &&
                particles.PointData().size() / 3 == particles.ForceData().size() / 3) {
                spdlog::error("Error whilst reading {}: data array sizes do not match", filename);
                exit(EXIT_FAILURE);
            }

            unsigned particle_count = particles.MassData().size();
            sim.container.particles.reserve(sim.container.particles.size() + particle_count);
            for (unsigned i = 0; i < particle_count; i++) {
                Particle p { { particles.PointData()[i * 3],
                               particles.PointData()[i * 3 + 1],
                               particles.PointData()[i * 3 + 2] },
                             { particles.VelData()[i * 3],
                               particles.VelData()[i * 3 + 1],
                               particles.VelData()[i * 3 + 2] },
                             particles.MassData()[i],
                             (int)particles.TypeData()[i],
                             i,
                             sim.stationaryParticleTypes.find((int)particles.TypeData()[i]) !=
                                 sim.stationaryParticleTypes.end() };
                p.setF({ particles.ForceData()[i * 3],
                         particles.ForceData()[i * 3 + 1],
                         particles.ForceData()[i * 3 + 2] });
                p.setOldF({ particles.OldForceData()[i * 3],
                            particles.OldForceData()[i * 3 + 1],
                            particles.OldForceData()[i * 3 + 2] });
                sim.container.addParticle(p);
            }
            spdlog::info("Read {} particles indiviually from particle data.", particle_count);
        }

        const auto& membranes = sim_input->clusters().membrane();
        for(auto& membrane : membranes){
            // TODO: Read in membrane
        }

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
