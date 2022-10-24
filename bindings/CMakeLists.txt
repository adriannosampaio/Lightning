find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)

pybind11_add_module(pyLightning domain.cpp)

#target_include_directories(Graphics PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
#target_link_libraries(Graphics PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Eigen3::Eigen)
target_include_directories(pyLightning PUBLIC .)
target_link_libraries(pyLightning PRIVATE Core Eigen3::Eigen)
    