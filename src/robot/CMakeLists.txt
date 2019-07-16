add_library(robot
    basic_robot.cpp
    basic_connection.cpp
    connection_factory.cpp
    # send_wait_event_state.cpp

    x51_connection.cpp
    x51_netclient.cpp
    x51_connection_factory.cpp
    x51_netclient_manager.cpp
)
target_link_libraries(robot Qt5::Core env_logger)
target_include_directories(robot PUBLIC ${PROJ_INCLUDE_DIRS} ${nlohmann-json_SOURCE_DIR})
target_compile_definitions(robot PUBLIC STAR_SERVER _SERVER_INVERTORY_SYS QT_NO_KEYWORDS)
target_compile_options(robot PUBLIC -Wno-deprecated-declarations -Wno-format -Wno-narrowing -include limits.h)

add_executable(robot_test
    x51_connection_test.cpp
    ${PROJ_SOURCES}
    target_projs/x51/dummy_src/QuestTemplate.cpp
)
target_link_libraries(robot_test robot pthread Qt5::Core dl gtest_main gtest)
add_test(robot_test robot_test)