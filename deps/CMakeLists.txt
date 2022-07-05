file(GLOB SOURCES *.hpp)
add_library (SimpleArgParser INTERFACE ${SOURCES})
target_include_directories(SimpleArgParser INTERFACE .)
