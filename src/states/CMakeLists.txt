
set(SRCS

autogen/login/state_ConnectGame.cpp
impl/login/state_ConnectGame_impl.cpp

autogen/login/state_DisconnectGame.cpp
impl/login/state_DisconnectGame_impl.cpp

autogen/login/state_GameConnected.cpp
impl/login/state_GameConnected_impl.cpp

autogen/login/state_CEventConnectRes.cpp
impl/login/state_CEventConnectRes_impl.cpp

autogen/login/state_CEventRefreshPlayerInfo.cpp
impl/login/state_CEventRefreshPlayerInfo_impl.cpp

autogen/login/state_CEventClientLoadAccountInfo.cpp
impl/login/state_CEventClientLoadAccountInfo_impl.cpp

autogen/login/state_CEventEditRole.cpp
impl/login/state_CEventEditRole_impl.cpp

autogen/login/state_CEventChooseRole.cpp
impl/login/state_CEventChooseRole_impl.cpp

autogen/login/state_Logined.cpp
impl/login/state_Logined_impl.cpp

autogen/autochess/state_CEventAutoChessStartCoor.cpp
impl/autochess/state_CEventAutoChessStartCoor_impl.cpp

autogen/autochess/state_CEventAutoChessCoorResult.cpp
impl/autochess/state_CEventAutoChessCoorResult_impl.cpp

autogen/autochess/state_CEventAutoChessNotifyClientCreateMatchRes.cpp
impl/autochess/state_CEventAutoChessNotifyClientCreateMatchRes_impl.cpp

autogen/autochess/state_CEventAutoChessStopCoor.cpp
impl/autochess/state_CEventAutoChessStopCoor_impl.cpp

autogen/state_factory.cpp
)
    
add_library(states ${SRCS})
target_link_libraries(states Qt5::Core env_logger net_x51 testcase)
target_include_directories(states PUBLIC ${nlohmann-json_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/autogen)
target_compile_definitions(states PUBLIC JSON_MultipleHeaders=ON)
target_compile_options(states PRIVATE 
    -Wno-unknown-pragmas -Wno-unused-parameter -Wno-pedantic -Wno-missing-field-initializers 
    -Wno-unused-but-set-variable -Wno-sign-compare -Wno-misleading-indentation -Wno-unused-value
    -Wno-unused-variable -Wno-delete-non-virtual-dtor -Wno-reorder -Wno-ignored-qualifiers
    -Wno-unused-function -Wno-parentheses -Wno-shift-negative-value
)
