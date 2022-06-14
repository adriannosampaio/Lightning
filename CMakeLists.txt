cmake_minimum_required(VERSION 3.10)

# set the project name
project(Lightning)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# find relevant packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(Eigen3 3.0.4 REQUIRED)

# add tests 
add_subdirectory(tests/unit_tests)

# graphics library
add_subdirectory(libs/graphics)

file(GLOB SOURCES src/main.cpp)
add_executable (Lightning ${SOURCES})
target_include_directories(Lightning PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Lightning PUBLIC  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_include_directories(Lightning PRIVATE libs)
target_link_libraries(Lightning PUBLIC Eigen3::Eigen SDL_Wrapper)


install(TARGETS Lightning DESTINATION bin)
#install(DIRECTORY tests/ DESTINATION tests)
install(TARGETS test_project DESTINATION unit_tests)