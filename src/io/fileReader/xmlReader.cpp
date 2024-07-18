
#include "io/fileReader/xmlReader.h"
#include "io/xsd/simulation.h"
#include "models/generators/ParticleGenerator.h"
#include "models/molecules/Membrane.h"
#include "simulation/MembraneSimulation.h"
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

        if (sim_input->molecules().present()) {
            size_t moleculesId = 1;
            if (sim_input->molecules().get().membrane().size()) {
                try {
                    // downcast to membraneSim
                    MembraneSimulation& membraneSim = dynamic_cast<MembraneSimulation&>(sim);
                    for (auto membrane : sim_input->molecules().get().membrane()) {
                        std::array<double, 3> pos = { membrane.pos().x(),
                                                      membrane.pos().y(),
                                                      membrane.pos().z() };
                        std::array<double, 3> vel = { membrane.vel().x(),
                                                      membrane.vel().y(),
                                                      membrane.vel().z() };
                        unsigned ptype = 0;
                        if (membrane.ptype().present())
                            ptype = membrane.ptype().get();
                        membraneSim.molecules.push_back(std::make_unique<Membrane>(
                            Membrane { pos,
                                       membrane.dim().x(),
                                       membrane.dim().y(),
                                       membrane.dim().z(),
                                       membrane.spacing(),
                                       membrane.mass(),
                                       vel,
                                       membrane.brownVel(),
                                       static_cast<size_t>(membrane.brownDim()),
                                       ptype,
                                       membrane.equiDist(),
                                       membrane.springConst() }));
                        spdlog::info(
                            "Registered membrane with id {}: pos = ({}, {}, {}), vel = ({}, {}, "
                            "{}), mass = {}, spacing = {}, brownVel = {}, brownDim = {}, ptype = "
                            "{}, springConst = {}, equiDist = {}",
                            moleculesId,
                            pos[0],
                            pos[1],
                            pos[2],
                            vel[0],
                            vel[1],
                            vel[2],
                            membrane.mass(),
                            membrane.spacing(),
                            membrane.brownVel(),
                            static_cast<size_t>(membrane.brownDim()),
                            ptype,
                            membrane.equiDist(),
                            membrane.springConst());
                    }
                } catch (const std::bad_cast& e) {
                    spdlog::warn(
                        "Membrane was specified in xml, but can not generate with this simulation "
                        "type. Membrane will be ignored.");
                }
            }
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
