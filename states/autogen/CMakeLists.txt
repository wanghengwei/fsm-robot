
set(SRCS

state_ConnectGame.cpp
../impl/state_ConnectGame_impl.cpp

state_DisconnectGame.cpp
../impl/state_DisconnectGame_impl.cpp

state_CEventLogin.cpp
../impl/state_CEventLogin_impl.cpp

state_CEventHeart.cpp
../impl/state_CEventHeart_impl.cpp

state_InRoom.cpp
../impl/state_InRoom_impl.cpp

)
    
add_library(states_gen ${SRCS})
target_link_libraries(states_gen Qt5::Core env_logger robot)
target_include_directories(states_gen PUBLIC ${nlohmann-json_SOURCE_DIR})
target_compile_definitions(states_gen PUBLIC JSON_MultipleHeaders=ON)
