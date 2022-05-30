
# These tests can use the Catch2-provided main
add_executable(test_project test.cpp)
target_link_libraries(test_project PRIVATE Catch2::Catch2)
