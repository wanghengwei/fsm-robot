add_library(testcase
    basic_state.cpp
    testcase.cpp
    testcase_manager.cpp
    idle.cpp

    chart_parser.cpp
    xdoc_manager.cpp
)
target_link_libraries(testcase net_base pugixml)
target_include_directories(testcase PUBLIC ${nlohmann-json_SOURCE_DIR})

add_executable(testcase_test
    testcase_test.cpp
    testcase_manager_test.cpp
    chart_parser_test.cpp
)
target_link_libraries(testcase_test testcase gtest gtest_main states dl)
add_test(testcase_test testcase_test)