#include <iostream>
#include <map>

/**
 * @brief new class for testing
 *
 */
class MyClass {
   protected:
    /// my variable
    int num;
    /// Meu casal
    std::string name;
};

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
