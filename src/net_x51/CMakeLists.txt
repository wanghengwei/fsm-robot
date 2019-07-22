set_source_files_properties(
    x51_connection.cpp
    x51_netclient.cpp
    x51_netclient_manager.cpp
    PROPERTIES
    COMPILE_OPTIONS "-Wno-missing-field-initializers;-Wno-unused-parameter;-Wno-unused-value;-Wno-unknown-pragmas;-Wno-unused-but-set-variable;-Wno-misleading-indentation;-Wno-sign-compare;-Wno-unused-variable;-Wno-format;-Wno-delete-non-virtual-dtor"
)

add_library(net_x51
    x51_connection.cpp
    x51_netclient.cpp
    x51_connection_factory.cpp
    x51_netclient_manager.cpp
)
target_link_libraries(net_x51 net_base Qt5::Core env_logger)
target_include_directories(net_x51 PUBLIC ${PROJ_INCLUDE_DIRS})
target_compile_definitions(net_x51 PUBLIC STAR_SERVER _SERVER_INVERTORY_SYS QT_NO_KEYWORDS)
# target_compile_options(net_x51 PUBLIC -Wno-deprecated-declarations -Wno-format -Wno-narrowing -include limits.h)

# add_executable(net_x51_test
#     x51_connection_test.cpp
#     ${PROJ_SOURCES}
# )
# target_link_libraries(net_x51_test net_x51 pthread Qt5::Core dl gtest_main gtest)
# target_compile_options(net_x51_test PRIVATE "-w")
# add_test(net_x51_test net_x51_test)