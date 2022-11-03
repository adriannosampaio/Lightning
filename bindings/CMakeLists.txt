find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)

pybind11_add_module(pyLightning domain.cpp)
set_target_properties(pyLightning PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(pyLightning PROPERTIES SUFFIX ".pyd")
target_include_directories(pyLightning PUBLIC .)
target_link_libraries(pyLightning PRIVATE Core Eigen3::Eigen)
install(TARGETS pyLightning DESTINATION bin)    
