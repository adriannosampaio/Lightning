#include <Eigen/Dense>
#include <SimpleArgParser/SimpleArgParser.hpp>
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "Domain.hpp"
#include "DomainParameters.hpp"
#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "Utils.hpp"
#include "Graphics.hpp"

int main(int argc, char** argv) {
    graphics::initialize();

    parser::ArgumentParser parser(argc, argv);
    parser.addArgument("-f", true, false, "input light file");
    parser.addArgument("-o", true, false, "Output png file");

    parser.addArgument(
        "-S", true, false, "Size of each lightning segment in number of ");
    parser.addArgument(
        "-M",
        true,
        false,
        "Number of preliminary candidates generated per point");
    parser.addArgument(
        "-N", true, false, "Max number of true candidates accepted per point");
    parser.addArgument("-I", true, false, "Max number of iterations");
    parser.addArgument(
        "-R", true, false, "Minimum distance between true candidate points");
    parser.addArgument(
        "-D",
        true,
        false,
        "Minimum accepted distance between a step leader and the end point");
    parser.addArgument(
        "--noise",
        true,
        false,
        "Minimum accepted distance between a step leader and the end point");
    try {
        parser.parse();

        auto [domain, params] = setup_domain(parser.getArgument<std::string>("-f"));
        if (parser.isDefined("-S"))
            params->lightning_segment_size = parser.getArgument<double>("-S");
        if (parser.isDefined("-M"))
            params->new_points_per_leader = parser.getArgument<unsigned>("-M");
        if (parser.isDefined("-N"))
            params->maximum_accepted_candidates =
                parser.getArgument<unsigned>("-N");
        if (parser.isDefined("-R"))
            params->minimum_candidate_distance =
                parser.getArgument<double>("-R");
        if (parser.isDefined("-I"))
            params->max_iterations = parser.getArgument<double>("-I");
        if (parser.isDefined("-D"))
            params->accepted_distance_to_end_point =
                parser.getArgument<double>("-D");

        double noise = 0.2;
        if (parser.isDefined("--noise"))
            noise = parser.getArgument<double>("--noise");
        params->noise = noise;

        std::shared_ptr<graphics::Renderer> renderer =
            std::make_shared<graphics::Renderer>();

        auto l = domain->generate_path(*params);
        for (auto line : graphics::get_lines_simple(*params, l->get_root())) renderer->add_renderable(line);
        // Generate an image
        if (parser.isDefined("-o"))
            renderer->generate_image(
                parser.getArgument<std::string>("-o"), 800, 600);
        else
            renderer->run("Window", 800, 600);

        std::map<std::string, int> m;
        m["test"] = 1;
        m["test2"] = 4;

        for (auto& [name, total] : m) {  // cppcheck-suppress unassignedVariable
            std::cout << name << " - " << total << "\n";
        }
        std::cout << "Project test\n";
    } catch (parser::parsing_exception& e) {
        std::cerr << e.message();
    }

    graphics::finalize();
    return 0;
}
