add_library(net_x51
    # basic_robot.cpp
    # basic_connection.cpp
    # connection_factory.cpp
    # send_wait_event_state.cpp

    x51_connection.cpp
    x51_netclient.cpp
    x51_connection_factory.cpp
    x51_netclient_manager.cpp
)
target_link_libraries(net_x51 net_base Qt5::Core env_logger)
target_include_directories(net_x51 PUBLIC ${PROJ_INCLUDE_DIRS})
target_compile_definitions(net_x51 PUBLIC STAR_SERVER _SERVER_INVERTORY_SYS QT_NO_KEYWORDS)
target_compile_options(net_x51 PUBLIC -Wno-deprecated-declarations -Wno-format -Wno-narrowing -include limits.h)

add_executable(net_x51_test
    x51_connection_test.cpp
    ${PROJ_SOURCES}
    target_projs/x51/dummy_src/QuestTemplate.cpp
)
target_link_libraries(net_x51_test net_x51 pthread Qt5::Core dl gtest_main gtest)
add_test(net_x51_test net_x51_test)