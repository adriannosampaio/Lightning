#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <queue>
#include <vector>

#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "physics/DomainParameters.hpp"
#include "physics/ElectricCharge.hpp"
#include "physics/ElectricField.hpp"
#include "physics/Utils.hpp"

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

    std::shared_ptr<Lightning> generate_path() {
        int M = 60, N = 5, R = 15;

        auto middle_point_it = std::stable_partition(
            _charges.begin(),
            _charges.end(),
            [](const physics::ElectricCharge& q) -> bool {
                return q.charge < 0;
            });
        // Selecting a random starting point from
        int root_id =
            std::rand() % std::distance(_charges.begin(), middle_point_it);
        auto root_coords = _charges[root_id].position;
        auto root = std::make_shared<PathPoint>(
            root_coords[0], root_coords[1], root_coords[2]);

        bool reached_destination = false;
        std::queue<std::shared_ptr<PathPoint>> step_leaders_to_explore;
        step_leaders_to_explore.push(root);
        while (!step_leaders_to_explore.empty() && !reached_destination) {
            auto current_leader = step_leaders_to_explore.front();
            step_leaders_to_explore.pop();

            auto leader_position = current_leader->get_position();
            double leader_potential = _field->get_potential(leader_position);
            auto heap_function = [&](std::shared_ptr<PathPoint> ptr0,
                                     std::shared_ptr<PathPoint> ptr1) {
                auto candidate_position_0 = ptr0->get_position();
                auto candidate_potential_0 =
                    _field->get_potential(candidate_position_0);
                double field_value_0 =
                    (candidate_potential_0 - leader_potential) /
                    physics::get_euclidean_distance(
                        candidate_position_0, leader_position);
                auto candidate_position_1 = ptr1->get_position();
                auto candidate_potential_1 =
                    _field->get_potential(candidate_position_1);
                double field_value_1 =
                    (candidate_potential_1 - leader_potential) /
                    physics::get_euclidean_distance(
                        candidate_position_1, leader_position);
                return field_value_0 < field_value_1;
            };

            // Generate random points
            std::vector<std::shared_ptr<PathPoint>> candidates;
            candidates.reserve(M);
            for (int point_idx = 0; point_idx < M; point_idx++) {
                auto color = graphics::WHITE;
                Eigen::Vector3d coords =
                    leader_position + physics::generate_random_point_in_sphere(
                                          4 * _parameters.max_cell_dimension);
                if (!_parameters.is_inside(coords)) {
                    coords = _parameters.clamp_coordinates_to_boundary(coords);
                }
                if (_field->get_potential(coords) < leader_potential) {
                    continue;
                }
                candidates.push_back(std::make_shared<PathPoint>(coords));
                std::push_heap(
                    candidates.begin(), candidates.end(), heap_function);
            }

            if (candidates.size() == 0) continue;
            // Get N candidates
            std::vector<std::shared_ptr<PathPoint>> step_leaders;
            std::pop_heap(candidates.begin(), candidates.end(), heap_function);
            step_leaders.push_back(candidates.back());
            candidates.pop_back();
            for (int candidate_idx = 0;
                 candidate_idx < N - 1 && candidates.size() > 0;
                 candidate_idx++) {
                std::pop_heap(
                    candidates.begin(), candidates.end(), heap_function);
                auto current_candidate = candidates.back();
                candidates.pop_back();
                auto current_pos = current_candidate->get_position();
                bool skip = false;
                for (auto selected_candidate : step_leaders) {
                    auto selected_pos = selected_candidate->get_position();
                    double dist = physics::get_euclidean_distance(
                        current_pos, selected_pos);
                    skip = dist < R;
                }
                if (!skip) {
                    step_leaders.push_back(current_candidate);
                }
            }

            for (auto selected_candidate : step_leaders) {
                current_leader->add_child(selected_candidate);
                double min_distance_to_end = HUGE_VAL;
                for (auto charge_it = middle_point_it;
                     charge_it != _charges.end();
                     charge_it++) {
                    double dist_to_end = physics::get_euclidean_distance(
                        selected_candidate->get_position(),
                        charge_it->get_position());
                    if (dist_to_end < min_distance_to_end)
                        min_distance_to_end = dist_to_end;
                }
                if (min_distance_to_end < 20 * _parameters.max_cell_dimension) {
                    reached_destination = true;
                }
                step_leaders_to_explore.push(selected_candidate);
            }
        }

        // root->add_child(std::make_shared<PathPoint>(200, 0, 200));
        // root->add_child(std::make_shared<PathPoint>(100, 0, 200));
        // root->get_child(1)->add_child(std::make_shared<PathPoint>(150, 0,
        // 300)); root->get_child(1)->add_child(std::make_shared<PathPoint>(150,
        // 0, 500));
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
                DomainParameters(dimensions, num_cells));
            domain_created = true;
        }
    }
    return domain;
}
