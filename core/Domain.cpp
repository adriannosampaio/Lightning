#include "Domain.hpp"
#include "DomainParameters.hpp"

std::shared_ptr<Lightning> Domain::generate_path(const DomainParameters& params) {
    int M = params.new_points_per_leader,
        N = params.maximum_accepted_candidates;

    double R = params.minimum_candidate_distance,
           segment_length = params.lightning_segment_size,
           distance_to_end = params.accepted_distance_to_end_point;

    auto middle_point_it = std::stable_partition(
        _charges.begin(),
        _charges.end(),
        [](const core::ElectricCharge& q) -> bool { return q.is_negative; });
    _positive_charges_start = middle_point_it;
    // Selecting a random starting point from
    int root_id =
        std::rand() % std::distance(_charges.begin(), middle_point_it);
    auto root_coords = _charges[root_id].position;
    auto root = std::make_shared<PathPoint>(
        root_coords[0], root_coords[1], root_coords[2]);

    bool reached_destination = false;
    std::queue<std::shared_ptr<PathPoint>> step_leaders_to_explore;
    std::shared_ptr<PathPoint> final_step_leader;
    root->level = 0;
    step_leaders_to_explore.push(root);
    unsigned iter = 0, maximum_level = 0;
    while (!step_leaders_to_explore.empty() && !reached_destination &&
           iter++ < params.max_iterations) {
        // double leader_potential = _field->get_potential(leader_position);
        auto heap_function = [&](std::shared_ptr<PathPoint> ptr0,
                                 std::shared_ptr<PathPoint> ptr1) {
            double weight_0 = ptr0->get_weight();
            double weight_1 = ptr1->get_weight();
            return weight_0 < weight_1;
        };

        // Generate random points
        // std::cout << "Generating preliminary candidates\n";
        std::vector<std::shared_ptr<PathPoint>> candidates;
        size_t num_leaders = step_leaders_to_explore.size();
        candidates.reserve(M * num_leaders);

        for (int leader_point = 0; leader_point < num_leaders; leader_point++) {
            auto current_leader = step_leaders_to_explore.front();
            auto leader_position = current_leader->get_position();
            step_leaders_to_explore.pop();
            auto points = core::generate_fibonacci_sphere(
                leader_position, segment_length, M);
            for (auto& point : points) {
                double rand_float = static_cast<double>(rand()) / RAND_MAX;
                double noise_variance =
                    params.noise ? 1 + fmod(rand_float, params.noise) : 1;

                auto next_candidate = std::make_shared<PathPoint>(point);
                double min_dist =
                    this->get_smallest_distance_to_end_point(point);
                double weight = 1000 / min_dist * noise_variance;
                next_candidate->weight = weight;
                next_candidate->parent = current_leader;
                next_candidate->level = current_leader->level + 1;
                candidates.push_back(next_candidate);
                maximum_level =
                    std::max(candidates.back()->level, maximum_level);
                // std::cout << "\t" << point.transpose() << " -> " << weight
                //        << "\n";
                std::push_heap(
                    candidates.begin(), candidates.end(), heap_function);
            }
        }
        // for (auto& p : candidates) {
        //    std::cout << p->get_weight() << "  ";
        //}
        // std::cout << "\n";

        if (candidates.size() == 0) continue;
        // Get N candidates
        std::vector<std::shared_ptr<PathPoint>> step_leaders;
        std::pop_heap(candidates.begin(), candidates.end(), heap_function);
        step_leaders.push_back(candidates.back());
        candidates.pop_back();
        for (int candidate_idx = 0;
             candidate_idx < N - 1 && candidates.size() > 0;
             candidate_idx++) {
            std::pop_heap(candidates.begin(), candidates.end(), heap_function);
            auto current_candidate = candidates.back();
            candidates.pop_back();
            auto current_pos = current_candidate->get_position();
            // std::cout << "Visiting true candidate on: "
            //          << current_pos.transpose() << "\n";
            bool skip = false;
            for (auto selected_candidate : step_leaders) {
                auto selected_pos = selected_candidate->get_position();
                double dist =
                    core::get_euclidean_distance(current_pos, selected_pos);
                // std::cout
                //    << "\ttesting against: " << selected_pos.transpose()
                //    << " : dist =" << dist << "\n";
                skip = dist < R;
            }
            if (!skip) {
                step_leaders.push_back(current_candidate);
            }
        }

        for (auto selected_candidate : step_leaders) {
            if (selected_candidate->get_parent())
                selected_candidate->get_parent()->add_child(selected_candidate);
            double min_distance_to_end =
                this->get_smallest_distance_to_end_point(
                    selected_candidate->get_position());
            if (min_distance_to_end < distance_to_end) {
                reached_destination = true;
                final_step_leader = selected_candidate;
            }
            step_leaders_to_explore.push(selected_candidate);
        }
    }

    // Finish extra information
    auto pointer = final_step_leader;
    if (!reached_destination) {
        pointer = step_leaders_to_explore.front();
        step_leaders_to_explore.pop();
        while (!step_leaders_to_explore.empty()) {
            auto next = step_leaders_to_explore.front();
            step_leaders_to_explore.pop();
            if (pointer->weight < next->weight) {
                pointer = next;
            }
        }
    }
    pointer->is_main_branch = true;
    while (pointer = pointer->parent) {
        pointer->is_main_branch = true;
    }

    auto res = std::make_shared<Lightning>(root);
    res->max_level = maximum_level;
    return res;
}

void Domain::generate_paths(const DomainParameters& params, unsigned number_of_paths) {
    // static_assert(number_of_paths > 0);
    _lightning_paths.reserve(number_of_paths);
    for (unsigned i = 0; i < number_of_paths; i++) {
        _lightning_paths.push_back(generate_path(params));
    }
}

std::pair<std::shared_ptr<Domain>, std::shared_ptr<DomainParameters>>
    setup_domain(const std::string& filename)
{
    std::ifstream file(filename);
    std::string data;
    bool filled_dimensions = false;
    Eigen::Vector3d dimensions;
    bool filled_cells = false;
    Eigen::Vector3i num_cells;
    bool domain_created = false;
    std::shared_ptr<Domain> domain = nullptr;
    std::shared_ptr<DomainParameters> params = nullptr;
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
            domain->add_charge(Eigen::Vector3d{x, y, z}, q < 0);
        }

        if (filled_dimensions && filled_cells && !domain_created) {
            domain = std::make_shared<Domain>();
            params = std::make_shared<DomainParameters>(dimensions, num_cells);
            domain_created = true;
        }
    }
    return {domain, params};
}
