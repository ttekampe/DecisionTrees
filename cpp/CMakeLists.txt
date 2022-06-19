cmake_minimum_required ( VERSION 3.23.2 )

project(
    DecisionTree 
    VERSION 1.0
    DESCRIPTION "An implementation of decision trees"
    LANGUAGES CXX
)

include(CTest)
enable_testing()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Boost 1.78 COMPONENTS unit_test_framework REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")

add_subdirectory(src)
add_subdirectory(tests)

include_directories(${CMAKE_SOURCE_DIR}/src)