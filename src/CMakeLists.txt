cmake_minimum_required(VERSION 3.12)

project(robot C CXX)

enable_testing()

# 如果ccache存在，就使用ccache
# 如果要使用distcc，需要自己设置环境变量 CCACHE_PREFIX=distcc
find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message("use ccache as c++ compuler launcher")
endif()

# find_program(CPPCHECK cppcheck)
# if(CPPCHECK)
#     set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
#     message("use cppcheck: ${CPPCHECK}")
# endif()

include(ExternalProject)
ExternalProject_Add(ep-gtest
    URL https://github.com/google/googletest/archive/release-1.8.1.zip
    URL_MD5 ad6868782b5952b7476a7c1c72d5a714
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ep
)

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
    # message("${nlohmann-json_SOURCE_DIR}")
endif()

## 项目(x51/mgc)
set(PROJ "x51" CACHE STRING "x51 or mgc")
set(ALL_PROJS x51 mgc)
if (NOT PROJ IN_LIST ALL_PROJS)
    message(FATAL_ERROR "-DPROJ=x51 or mgc")
endif()
message("PROJ=${PROJ}")

include(target_projs/x51/proj.cmake)

add_subdirectory(net_base)
add_subdirectory(net_x51)
add_subdirectory(testcase)
add_subdirectory(states)

# add_library(base 
#     testcase.cpp
#     basic_state.cpp
#     testcase_manager.cpp
#     # state_factory.cpp
# )
# target_link_libraries(base Qt5::Core Qt5::Xml pugixml net_x51 states env_logger dl)
# target_include_directories(base PUBLIC ${nlohmann-json_SOURCE_DIR})

add_executable(demo 
    main.cpp 
    ${PROJ_SOURCES}
    target_projs/x51/dummy_src/QuestTemplate.cpp
)
target_link_libraries(demo testcase net_x51 states dl)
target_compile_options(demo PRIVATE 
    -Wno-unknown-pragmas -Wno-unused-parameter -Wno-pedantic -Wno-missing-field-initializers 
    -Wno-unused-but-set-variable -Wno-sign-compare -Wno-misleading-indentation -Wno-unused-value
    -Wno-unused-variable -Wno-delete-non-virtual-dtor -Wno-reorder -Wno-ignored-qualifiers
    -Wno-unused-function -Wno-parentheses
)
# target_include_directories(demo PRIVATE ${PROJ_INCLUDE_DIRS})
# target_compile_definitions(demo PRIVATE STAR_SERVER _SERVER_INVERTORY_SYS QT_NO_KEYWORDS)
# target_compile_options(demo PRIVATE -Wno-deprecated-declarations -Wno-format -Wno-narrowing -include limits.h)

# add_executable(base_test testcase_manager_test.cpp testcase_test.cpp)
# target_link_libraries(base_test gtest_main gtest base)

# add_test(NAME base_test COMMAND base_test)