add_library(spdlog spdlog.cpp)
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)
target_link_libraries(spdlog pthread)

add_library(env_logger logger.cpp parser.cpp)
target_link_libraries(env_logger Qt5::Core spdlog)
target_include_directories(env_logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(env_logger_test parser_test.cpp logger_test.cpp)
target_link_libraries(env_logger_test env_logger gtest gtest_main pthread Qt5::Core)
add_dependencies(env_logger_test ep-gtest)

add_test(NAME env_logger_test COMMAND env_logger_test)