
file(GLOB SOURCES *.cpp *.hpp)
add_library (Core STATIC ${SOURCES})


#target_include_directories(Graphics PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
#target_link_libraries(Graphics PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Eigen3::Eigen)
set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(Core PUBLIC .)
target_link_libraries(Core PRIVATE Eigen3::Eigen)
