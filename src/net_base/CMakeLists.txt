add_library(net_base
    basic_robot.cpp
    basic_connection.cpp
    connection_factory.cpp
)
target_link_libraries(net_base Qt5::Core env_logger)