cmake_minimum_required(VERSION 3.12)

project(robot C CXX)

enable_testing()

include(ExternalProject)
ExternalProject_Add(ep-gtest
    URL https://github.com/google/googletest/archive/release-1.8.1.zip
    URL_MD5 ad6868782b5952b7476a7c1c72d5a714
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ep
)
# find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/ep/include)
link_directories(${CMAKE_BINARY_DIR}/ep/lib)

find_package(Qt5 COMPONENTS Core Xml REQUIRED)

add_subdirectory(env_logger)

add_subdirectory(states)

add_executable(demo main.cpp robot.cpp testcase.cpp
    basic_state.cpp
    testcase_manager.cpp
    state_factory.cpp
)
target_link_libraries(demo Qt5::Core Qt5::Xml pugixml states env_logger)
# target_compile_definitions(demo PUBLIC SPDLOG_COMPILED_LIB)

add_test(NAME all_test COMMAND env_logger_test)