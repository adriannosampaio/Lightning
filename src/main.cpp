#include <iostream>
#include <map>
int main() {
    std::map<std::string, int> m;
    m["test"] = 1;
    m["test2"] = 4;

    for (auto& [name, total] : m) {  // cppcheck-suppress unassignedVariable
        std::cout << name << " - " << total << "\n";
    }
    std::cout << "Project test\n";
    return 0;
}
