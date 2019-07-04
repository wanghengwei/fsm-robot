cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR})
find_package(Qt5 COMPONENTS Core Xml REQUIRED)

add_subdirectory(states)

add_executable(demo main.cpp robot.cpp testcase.cpp
    basic_state.cpp
    testcase_manager.cpp
    state_factory.cpp
    # states/ConnectGame.cpp
    # states/CEventLogin.cpp
    # states/CEventLogin_impl.cpp
)
target_link_libraries(demo Qt5::Core Qt5::Xml pugixml states)