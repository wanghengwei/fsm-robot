
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

state_Logined.cpp
../impl/state_Logined_impl.cpp

state_factory.cpp
)
    
add_library(states ${SRCS})
target_link_libraries(states Qt5::Core env_logger net_x51 testcase)
target_include_directories(states PUBLIC ${nlohmann-json_SOURCE_DIR})
target_compile_definitions(states PUBLIC JSON_MultipleHeaders=ON)
target_compile_options(states PRIVATE 
    -Wno-unknown-pragmas -Wno-unused-parameter -Wno-pedantic -Wno-missing-field-initializers 
    -Wno-unused-but-set-variable -Wno-sign-compare -Wno-misleading-indentation -Wno-unused-value
    -Wno-unused-variable -Wno-delete-non-virtual-dtor -Wno-reorder -Wno-ignored-qualifiers
    -Wno-unused-function -Wno-parentheses
)