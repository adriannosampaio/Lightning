//#define CATCH_CONFIG_MAIN
#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>

#include "DomainParameters.hpp"
unsigned int Factorial(unsigned int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Test DomainParameters", "[DomainParameters][[physics]") {
    // 2D cases

    // Simple
    DomainParameters d({10, 1, 10}, {4, 1, 4});
    auto cell_coord_from_coord = d.coords_to_cell({1.0, 0.5, 2.6});
    REQUIRE(cell_coord_from_coord[0] == 0);
    REQUIRE(cell_coord_from_coord[1] == 0);
    REQUIRE(cell_coord_from_coord[2] == 1);
    // Simple
    cell_coord_from_coord = d.coords_to_cell({7.5, 0.5, 2.6});
    REQUIRE(cell_coord_from_coord[0] == 3);
    REQUIRE(cell_coord_from_coord[1] == 0);
    REQUIRE(cell_coord_from_coord[2] == 1);
    // Out of Bounds
}
TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Eigen", "[Eigen]") { Eigen::Matrix3d m; }