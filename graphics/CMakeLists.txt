
file(GLOB SOURCES *.cpp *.hpp)
add_library (Graphics STATIC ${SOURCES})


#target_include_directories(Graphics PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
#target_link_libraries(Graphics PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Eigen3::Eigen)
target_include_directories(Graphics PUBLIC .)
target_include_directories(Graphics PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Graphics PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Eigen3::Eigen)
