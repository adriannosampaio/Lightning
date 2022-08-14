#pragma once
#include <fstream>
#include <memory>
#include <vector>

#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "physics/DomainParameters.hpp"
#include "physics/ElectricCharge.hpp"

class Domain {
   private:
    std::shared_ptr<DomainParameters> _parameters;

    std::vector<std::shared_ptr<Lightning>> _lightning_paths;
    std::vector<physics::ElectricCharge> _charges;
    std::vector<double> _electric_potential_cells;

   public:
    explicit Domain(std::shared_ptr<DomainParameters> parameters) :
        _parameters(parameters) {}

    void set_parameters(std::shared_ptr<DomainParameters> parameters) {
        _parameters = parameters;
    }

    std::shared_ptr<DomainParameters> get_parameters() const {
        return _parameters;
    }
    void add_charge(const Eigen::Vector3d& position, double charge) {
        _charges.emplace_back(position, charge);
    }

    std::shared_ptr<Lightning> generate_path(int seed = 0) {
        auto root = std::make_shared<PathPoint>(100, 0, 100);
        root->add_child(std::make_shared<PathPoint>(200, 0, 200));
        root->add_child(std::make_shared<PathPoint>(100, 0, 200));
        root->get_child(1)->add_child(std::make_shared<PathPoint>(150, 0, 300));
        root->get_child(1)->add_child(std::make_shared<PathPoint>(150, 0, 500));
        return std::make_shared<Lightning>(root);
    }

    void generate_paths(unsigned number_of_paths) {
        // static_assert(number_of_paths > 0);
        _lightning_paths.reserve(number_of_paths);
        for (unsigned i = 0; i < number_of_paths; i++) {
            _lightning_paths.push_back(generate_path());
        }
    }

    ~Domain() {}
};

std::shared_ptr<Domain> setup_domain(const std::string& filename) {
    std::ifstream file(filename);
    std::string data;
    bool filled_dimensions = false;
    Eigen::Vector3d dimensions;
    bool filled_cells = false;
    Eigen::Vector3i num_cells;
    bool domain_created = false;
    std::shared_ptr<Domain> domain = nullptr;
    /*
    auto params = DomainParameters(
            Eigen::Vector3d{200, 1, 150}, Eigen::Vector3i{400, 1, 300});
        // Create domain
        Domain d(params);
        // Add electric charges
        d.add_charge(Eigen::Vector3d{10, 0, 10}, 10000);
        d.add_charge(Eigen::Vector3d{10, 0, 100}, 10000);
        d.add_charge(Eigen::Vector3d{150, 0, 100}, -10000);
    */
    while (file >> data) {
        if (data == "DIMS") {
            double x, y, z;
            file >> x >> y >> z;
            dimensions = Eigen::Vector3d{x, y, z};
            filled_dimensions = true;
        } else if (data == "CELLS") {
            int x, y, z;
            file >> x >> y >> z;
            num_cells = Eigen::Vector3i{x, y, z};
            filled_cells = true;
        } else if (data == "CHARGE") {
            if (!domain_created)
                throw std::runtime_error(
                    "Attempting to add charge to incomplete domain. "
                    "Please make sure the input file initializes both "
                    "the Domain's dimensions and number of cells before "
                    "adding a new charge");
            double x, y, z, q;
            file >> x >> y >> z >> q;
            domain->add_charge(Eigen::Vector3d{x, y, z}, q);
        }

        if (filled_dimensions && filled_cells && !domain_created) {
            domain = std::make_shared<Domain>(
                std::make_shared<DomainParameters>(dimensions, num_cells));
            domain_created = true;
        }
    }
    return domain;
}
