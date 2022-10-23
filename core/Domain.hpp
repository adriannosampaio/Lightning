#pragma once
#include <fstream>
#include <memory>
#include <vector>
#include <string>

#include "Lightning.hpp"
#include "PathPoint.hpp"

#include "ElectricCharge.hpp"

struct DomainParameters;

class Domain {
   private:

    std::vector<std::shared_ptr<Lightning>> _lightning_paths;
    std::vector<core::ElectricCharge> _charges;
    std::vector<core::ElectricCharge>::iterator _positive_charges_start;

   public:
    explicit Domain() {}

    void add_charge(const Eigen::Vector3d& position, double charge) {
        _charges.emplace_back(position, charge);
    }

    double get_smallest_distance_to_end_point(
        const Eigen::Vector3d& position) const {
        double min_distance_to_end = HUGE_VAL;
        for (auto charge_it = _positive_charges_start;
             charge_it != _charges.end();
             charge_it++) {
            double dist_to_end = core::get_euclidean_distance(
                position, charge_it->get_position());
            if (dist_to_end < min_distance_to_end)
                min_distance_to_end = dist_to_end;
        }
        return min_distance_to_end;
    }

    std::shared_ptr<Lightning> generate_path(const DomainParameters& params);

    void generate_paths(
        const DomainParameters& params, unsigned number_of_paths);

    ~Domain() {}
};

std::pair<std::shared_ptr<Domain>, std::shared_ptr<DomainParameters>>
    setup_domain(const std::string& filename);