
set(SRCS

state_ConnectGame.cpp
../impl/state_ConnectGame_impl.cpp

state_DisconnectGame.cpp
../impl/state_DisconnectGame_impl.cpp

state_GameConnected.cpp
../impl/state_GameConnected_impl.cpp

state_CEventConnectRes.cpp
../impl/state_CEventConnectRes_impl.cpp

state_CEventRefreshPlayerInfo.cpp
../impl/state_CEventRefreshPlayerInfo_impl.cpp

state_CEventClientLoadAccountInfo.cpp
../impl/state_CEventClientLoadAccountInfo_impl.cpp

state_CEventEditRole.cpp
../impl/state_CEventEditRole_impl.cpp

state_CEventChooseRole.cpp
../impl/state_CEventChooseRole_impl.cpp

)
    
add_library(states_gen ${SRCS})
target_link_libraries(states_gen Qt5::Core env_logger robot)
target_include_directories(states_gen PUBLIC ${nlohmann-json_SOURCE_DIR})
target_compile_definitions(states_gen PUBLIC JSON_MultipleHeaders=ON)
