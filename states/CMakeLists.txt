add_subdirectory(autogen)
add_library(states idle.cpp)
target_link_libraries(states states_gen Qt5::Core)
