#include <array>
#include <graphics/Graphics.hpp>
#include <iostream>
#include <map>
#include <memory>

int main() {
    graphics::initialize();

    std::shared_ptr<graphics::Renderer> renderer =
        std::make_shared<graphics::Renderer>(
            std::make_shared<graphics::Window>("window", 400, 600));

    renderer->add_renderable(std::make_shared<graphics::Line>(
        std::array<int, 2>{0, 0}, std::array<int, 2>{100, 100}));
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
