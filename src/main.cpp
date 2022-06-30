#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>

#include "Domain.hpp"
#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "graphics/Graphics.hpp"
#include "graphics/Renderable.hpp"
#include "physics/PhysicsUtils.hpp"

int main() {
    graphics::initialize();

    std::shared_ptr<graphics::Renderer> renderer =
        std::make_shared<graphics::Renderer>(
            std::make_shared<graphics::Window>("window", 400, 300));

    // auto line = std::make_shared<graphics::Line>(
    //     std::array<int, 2>{0, 0}, std::array<int, 2>{100, 100});

    auto params = DomainParameters(
        Eigen::Vector3d{200, 1, 150}, Eigen::Vector3i{400, 1, 300});
    Domain d(params);
    d.add_charge(Eigen::Vector3d{10, 0, 10}, 10000);
    d.add_charge(Eigen::Vector3d{10, 0, 100}, 10000);
    d.add_charge(Eigen::Vector3d{150, 0, 100}, -10000);
    d.generate_field();
    std::array<int, 2> grid_dimensions = {
        params.number_of_cells[0], params.number_of_cells[2]};
    auto field_data = d.get_field();
    auto f = std::make_shared<graphics::Field>(
        grid_dimensions, field_data, physics::MAX_POTENTIAL, 0, -1);
    // auto f = std::make_shared<graphics::Field>(grid_dimensions,
    // std::vector<double>(10000, 255), 1e10, 0, -1);
    renderer->add_renderable(f);
    renderer->run();

    std::map<std::string, int> m;
    m["test"] = 1;
    m["test2"] = 4;

    for (auto& [name, total] : m) {  // cppcheck-suppress unassignedVariable
        std::cout << name << " - " << total << "\n";
    }
    std::cout << "Project test\n";
    graphics::finalize();
    return 0;
}
