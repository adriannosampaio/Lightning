
add_executable(test_physics test_physics.cpp)
target_include_directories(test_physics PRIVATE ${PROJECT_SOURCE_DIR}/libs/physics)
target_link_libraries(test_physics PRIVATE Eigen3::Eigen Catch2::Catch2WithMain Physics)