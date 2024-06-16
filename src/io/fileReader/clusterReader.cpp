
#include "io/fileReader/clusterReader.h"
#include "models/generators/CuboidParticleCluster.h"
#include "models/generators/ParticleGenerator.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>

void ClusterReader::readFile(Simulation& sim)
{
    std::ifstream input_file(filename);
    std::string line;

    if (!input_file.is_open()) {
        spdlog::error("Error: could not open file {}", filename);
        exit(-1);
    }

    char clusterType;

    ParticleGenerator generator(sim.container);

    while (getline(input_file, line)) {
        spdlog::debug("Read line: {}", line);
        if (line.empty() || line.at(0) == '#')
            continue;
        std::istringstream iss(line);

        if (!(iss >> clusterType)) {
            spdlog::error("Could not read cluster type when reading file!");
            exit(EXIT_FAILURE);
        }

        switch (clusterType) {
        case 'C':
            double ox, oy, oz;
            int nx, ny, nz;
            double h, m, mv;
            double vx, vy, vz;
            double dim;
            if (!(iss >> ox >> oy >> oz >> nx >> ny >> nz >> h >> m >> mv >> vx >> vy >> vz >>
                  dim)) {
                spdlog::error(
                    "When reading cluster type Cuboid: Could not read parameters correctly");
                exit(EXIT_FAILURE);
            }
            generator.registerCluster(std::make_unique<CuboidParticleCluster>(
                CuboidParticleCluster({ ox, oy, oz }, nx, ny, nz, h, m, { vx, vy, vz }, mv, dim)));
            break;
        case 'S':
            double osx, osy, osz;
            size_t nr, dimS;
            double spacing, mass, brownianMotion;
            double ivx, ivy, ivz;
            size_t dimBrow;
            if (!(iss >> osx >> osy >> osz >> nr >> dimS >> spacing >> mass >> brownianMotion >>
                  ivx >> ivy >> ivz >> dimBrow)) {
                spdlog::error(
                    "When reading cluster type Sphere: Could not read parameters correctly");
                exit(EXIT_FAILURE);
            }
            generator.registerCluster(std::make_unique<SphereParticleCluster>(SphereParticleCluster(
                { osx, osy, osz },
                nr,
                dimS,
                spacing,
                mass,
                { ivx, ivy, ivz },
                brownianMotion,
                dimBrow)));
            break;
        default:
            spdlog::error("Invalid clusterType: {}", clusterType);
            exit(EXIT_FAILURE);
        }
    }

    generator.generateClusters();
}
