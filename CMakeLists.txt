cmake_minimum_required(VERSION 3.10)

# set the project name
project(Lightning)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# find relevant packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Eigen3 3.0.4 REQUIRED)

# dependency external libraries
add_subdirectory(deps)
# 
# graphics library
add_subdirectory(graphics)
add_subdirectory(core)
add_subdirectory(bindings)


file(GLOB SOURCES src/main.cpp)
add_executable (zeus ${SOURCES})
target_include_directories(zeus PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(zeus PUBLIC  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_include_directories(zeus PRIVATE libs core)
target_link_libraries(zeus PUBLIC Eigen3::Eigen Graphics Core SimpleArgParser)
SET(CMAKE_INSTALL_RPATH $ENV{CONDA_PREFIX}/Library/bin)
install(TARGETS zeus DESTINATION bin)
#message(file(GET_RUNTIME_DEPENDENCIES))
include(CPack)