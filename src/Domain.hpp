#pragma once
#include <memory>
#include <vector>

#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "physics/DomainParameters.hpp"
#include "physics/ElectricCharge.hpp"
#include "physics/ElectricField.hpp"

class Domain {
   private:
    DomainParameters _parameters;

    std::shared_ptr<physics::ElectricField> _field;
    std::vector<std::shared_ptr<Lightning>> _lightning_paths;
    std::vector<physics::ElectricCharge> _charges;
    std::vector<double> _electric_potential_cells;

   public:
    explicit Domain(DomainParameters parameters) :
        _parameters(parameters),
        _field(std::make_shared<physics::ElectricField>(parameters)) {}

    void set_parameters(DomainParameters parameters) {
        _parameters = parameters;
        _field = std::make_shared<physics::ElectricField>(parameters);
    }

    const DomainParameters& get_parameters() const { return _parameters; }

    void add_charge(const Eigen::Vector3d& position, double charge) {
        _charges.emplace_back(position, charge);
    }

    void generate_field() { _field->generate_field(_charges); }

    std::vector<double> get_field() { return _field->get_field_data(); }

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
