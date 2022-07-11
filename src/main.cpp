#include <Eigen/Dense>
#include <SimpleArgParser/SimpleArgParser.hpp>
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>

#include "Domain.hpp"
#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "graphics/Colors.hpp"
#include "graphics/Graphics.hpp"
#include "physics/Utils.hpp"

int main(int argc, char** argv) {
    graphics::initialize();

    parser::ArgumentParser parser(argc, argv);
    parser.addArgument("-f", true, true, "input light file");
    parser.addArgument("-o", true, true, "Output png file");
    try {
        parser.parse();

        std::shared_ptr<graphics::Renderer> renderer =
            std::make_shared<graphics::Renderer>();

        auto domain = setup_domain(parser.getArgument<std::string>("-f"));
        domain->generate_field();

        // Convert Electric field into a Field graphics object
        std::array<int, 2> grid_dimensions = {
            domain->get_parameters().number_of_cells[0],
            domain->get_parameters().number_of_cells[2]};
        auto field_data = domain->get_field();
        auto f = std::make_shared<graphics::Field>(
            grid_dimensions, field_data, physics::MAX_POTENTIAL, 0, -1);
        // Add the field graphics object to the renderer
        renderer->add_renderable(f);

        for (int i = 0; i < 100; i++) {
            auto color = graphics::WHITE;
            auto coords = physics::generate_random_point_in_sphere(100);
            std::array<int, 2> position = {
                static_cast<int>(coords[0]) + 200,
                static_cast<int>(coords[2]) + 150};

            auto p = std::make_shared<graphics::Pixel>(position, color);
            renderer->add_renderable(p);
        }

        auto l = domain->generate_path();
        for (auto line : l->get_lines()) renderer->add_renderable(line);
        // Generate an image
        renderer->generate_image(
            parser.getArgument<std::string>("-o"), 400, 300);

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
