add_library(testcase
    basic_state.cpp
    testcase.cpp
    testcase_manager.cpp
    idle.cpp
)
target_link_libraries(testcase net_base pugixml)
target_include_directories(testcase PUBLIC ${nlohmann-json_SOURCE_DIR})