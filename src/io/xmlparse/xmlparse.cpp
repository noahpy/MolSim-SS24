
#include "io/xmlparse/xmlparse.h"
#include "io/xsd/simulation.h"
#include <fstream>
#include <spdlog/spdlog.h>

void xmlparse(Params& sim_params, std::string& filename)
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
        if (params.start_time().present())
            sim_params.start_time = params.start_time().get();
        if (params.delta_t().present())
            sim_params.delta_t = params.delta_t().get();
        if (params.end_time().present())
            sim_params.end_time = params.end_time().get();
        if (params.frequency().present())
            sim_params.plot_frequency = params.frequency().get();
        if (params.epsilon().present())
            sim_params.epsilon = params.epsilon().get();
        if (params.sigma().present())
            sim_params.sigma = params.sigma().get();
        if (params.domainSize().present())
            sim_params.domain_size = { params.domainSize().get().x(),
                                       params.domainSize().get().y(),
                                       params.domainSize().get().z() };
        if (params.domainOrigin().present())
            sim_params.domain_origin = { params.domainOrigin().get().x(),
                                         params.domainOrigin().get().y(),
                                         params.domainOrigin().get().z() };
        if (params.cutoff().present())
            sim_params.cutoff = params.cutoff().get();
        if (params.output().present())
            sim_params.output_file = params.output().get();
        if (params.updateFreq().present())
            sim_params.update_frequency = params.updateFreq().get();
        if (params.boundaries().present()) {
            if (params.boundaries().get().bound_four().size()) {
                sim_params.boundaryConfig = BoundaryConfig(
                    getBoundaryType(params.boundaries().get().bound_four()[0]),
                    getBoundaryType(params.boundaries().get().bound_four()[1]),
                    getBoundaryType(params.boundaries().get().bound_four()[2]),
                    getBoundaryType(params.boundaries().get().bound_four()[3]));
            }
            if (params.boundaries().get().bound_six().size()) {
                sim_params.boundaryConfig = BoundaryConfig(
                    getBoundaryType(params.boundaries().get().bound_six()[0]),
                    getBoundaryType(params.boundaries().get().bound_six()[1]),
                    getBoundaryType(params.boundaries().get().bound_six()[2]),
                    getBoundaryType(params.boundaries().get().bound_six()[3]),
                    getBoundaryType(params.boundaries().get().bound_six()[4]),
                    getBoundaryType(params.boundaries().get().bound_six()[5]));
            }
        }

        // param checks
        if (sim_params.start_time > sim_params.end_time) {
            spdlog::error(
                "Error: start time ({}) must be less than end time ({})",
                sim_params.start_time,
                sim_params.end_time);
            exit(EXIT_FAILURE);
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
