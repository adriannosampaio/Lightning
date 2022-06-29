#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <map>
#include <memory>

#include "Domain.hpp"
#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "graphics/Graphics.hpp"
#include "graphics/Renderable.hpp"

int main() {
    graphics::initialize();

    std::shared_ptr<graphics::Renderer> renderer =
        std::make_shared<graphics::Renderer>(
            std::make_shared<graphics::Window>("window", 400, 600));

    // auto line = std::make_shared<graphics::Line>(
    //     std::array<int, 2>{0, 0}, std::array<int, 2>{100, 100});

    Domain d(DomainParameters(
        Eigen::Vector3d{100, 1, 100}, Eigen::Vector3i{10, 1, 10}));
    std::shared_ptr<Lightning> l = d.generate_path(1);
    d.add_charge(Eigen::Vector3d{10, 0, 10}, 1000);

    for (auto c : l->get_lines()) renderer->add_renderable(c);
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
