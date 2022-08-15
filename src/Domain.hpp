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
    std::vector<physics::ElectricCharge>::iterator _positive_charges_start;

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

    double get_smallest_distance_to_end_point(
        const Eigen::Vector3d& position) const {
        double min_distance_to_end = HUGE_VAL;
        for (auto charge_it = _positive_charges_start;
             charge_it != _charges.end();
             charge_it++) {
            double dist_to_end = physics::get_euclidean_distance(
                position, charge_it->get_position());
            if (dist_to_end < min_distance_to_end)
                min_distance_to_end = dist_to_end;
        }
        return min_distance_to_end;
    }

    std::shared_ptr<Lightning> generate_path(int seed = 0) {
        int M = _parameters->new_points_per_leader,
            N = _parameters->maximum_accepted_candidates;

        double R = _parameters->minimum_candidate_distance,
               segment_length = _parameters->lightning_segment_size,
               distance_to_end = _parameters->accepted_distance_to_end_point;

        auto middle_point_it = std::stable_partition(
            _charges.begin(),
            _charges.end(),
            [](const physics::ElectricCharge& q) -> bool {
                return q.charge < 0;
            });
        _positive_charges_start = middle_point_it;
        // Selecting a random starting point from
        int root_id =
            std::rand() % std::distance(_charges.begin(), middle_point_it);
        auto root_coords = _charges[root_id].position;
        auto root = std::make_shared<PathPoint>(
            root_coords[0], root_coords[1], root_coords[2]);

        bool reached_destination = false;
        std::queue<std::shared_ptr<PathPoint>> step_leaders_to_explore;
        step_leaders_to_explore.push(root);
        unsigned iter = 0;
        while (!step_leaders_to_explore.empty() && !reached_destination &&
               iter++ < _parameters->max_iterations) {
            auto current_leader = step_leaders_to_explore.front();
            step_leaders_to_explore.pop();

            auto leader_position = current_leader->get_position();
            // double leader_potential = _field->get_potential(leader_position);
            auto heap_function = [&](std::shared_ptr<PathPoint> ptr0,
                                     std::shared_ptr<PathPoint> ptr1) {
                double weight_0 = ptr0->get_weight();
                double weight_1 = ptr1->get_weight();
                return weight_0 < weight_1;
            };

            // Generate random points
            std::cout << "Generating preliminary candidates\n";
            std::vector<std::shared_ptr<PathPoint>> candidates;
            candidates.reserve(M);
            auto points = physics::generate_fibonacci_sphere(
                leader_position, segment_length, M);
            for (auto& point : points) {
                if (!_parameters->is_inside(point)) {
                    point = _parameters->clamp_coordinates_to_boundary(point);
                }
                double rand_float = static_cast<double>(rand()) / RAND_MAX;
                double noise_variance =
                    _parameters->noise ?
                        1 + fmod(rand_float, _parameters->noise) :
                        1;
                candidates.push_back(std::make_shared<PathPoint>(point));
                double min_dist =
                    this->get_smallest_distance_to_end_point(point);
                double weight = 1000 / min_dist * noise_variance;
                candidates.back()->set_weight(weight);
                std::cout << "\t" << point.transpose() << " -> " << weight
                          << "\n";
                std::push_heap(
                    candidates.begin(), candidates.end(), heap_function);
            }
            for (auto& p : candidates) {
                std::cout << p->get_weight() << "  ";
            }
            std::cout << "\n";

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
                std::cout << "Visiting true candidate on: "
                          << current_pos.transpose() << "\n";
                bool skip = false;
                for (auto selected_candidate : step_leaders) {
                    auto selected_pos = selected_candidate->get_position();
                    double dist = physics::get_euclidean_distance(
                        current_pos, selected_pos);
                    std::cout
                        << "\ttesting against: " << selected_pos.transpose()
                        << " : dist =" << dist << "\n";
                    skip = dist < R;
                }
                if (!skip) {
                    step_leaders.push_back(current_candidate);
                }
            }

            for (auto selected_candidate : step_leaders) {
                current_leader->add_child(selected_candidate);
                double min_distance_to_end =
                    this->get_smallest_distance_to_end_point(
                        selected_candidate->get_position());
                if (min_distance_to_end < distance_to_end) {
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
