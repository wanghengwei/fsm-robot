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

include(FetchContent)
FetchContent_Declare(nlohmann-json
    URL https://github.com/nlohmann/json/releases/download/v3.6.1/include.zip
    URL_MD5 0dc903888211db3a0f170304cd9f3a89
    # DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_GetProperties(nlohmann-json)
if(NOT nlohmann-json_POPULATED)
    FetchContent_Populate(nlohmann-json)
    message("${nlohmann-json_SOURCE_DIR}")
endif()

add_subdirectory(states)

add_library(base robot.cpp testcase.cpp
    basic_state.cpp
    testcase_manager.cpp
    state_factory.cpp
)
target_link_libraries(base Qt5::Core Qt5::Xml pugixml states env_logger)
# target_include_directories(base PUBLIC ${nlohmann-json_SOURCE_DIR})

add_executable(demo main.cpp)
target_link_libraries(demo base)
# target_include_directories(demo PUBLIC ${nlohmann-json_SOURCE_DIR}/nlohmann)
# target_compile_definitions(demo PUBLIC JSON_MultipleHeaders=ON)

add_executable(base_test testcase_manager_test.cpp testcase_test.cpp)
target_link_libraries(base_test gtest gtest_main base pthread)
# target_include_directories(base_test PRIVATE ${nlohmann-json_SOURCE_DIR})
# target_compile_definitions(base_test PRIVATE JSON_MultipleHeaders=ON)


add_test(NAME base_test COMMAND base_test)