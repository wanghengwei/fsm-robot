add_library(robot
    basic_robot.cpp
    basic_connection.cpp
    connection_factory.cpp
)
target_link_libraries(robot Qt5::Core env_logger)