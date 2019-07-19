
set(SRCS

autogen/login/state_ConnectGame.cpp
impl/login/state_ConnectGame_impl.cpp

autogen/login/state_DisconnectGame.cpp
impl/login/state_DisconnectGame_impl.cpp

autogen/login/state_GameConnected.cpp
impl/login/state_GameConnected_impl.cpp

autogen/login/state_WaitConnectRes.cpp
impl/login/state_WaitConnectRes_impl.cpp

autogen/login/state_RefreshPlayerInfo.cpp
impl/login/state_RefreshPlayerInfo_impl.cpp

autogen/login/state_LoadAccountInfo.cpp
impl/login/state_LoadAccountInfo_impl.cpp

autogen/login/state_EditRole.cpp
impl/login/state_EditRole_impl.cpp

autogen/login/state_ChooseRole.cpp
impl/login/state_ChooseRole_impl.cpp

autogen/login/state_Logined.cpp
impl/login/state_Logined_impl.cpp

autogen/autochess/state_StartCoor.cpp
impl/autochess/state_StartCoor_impl.cpp

autogen/autochess/state_WaitCoorResult.cpp
impl/autochess/state_WaitCoorResult_impl.cpp

autogen/autochess/state_NotifyClientCreateMatchRes.cpp
impl/autochess/state_NotifyClientCreateMatchRes_impl.cpp

autogen/autochess/state_StopCoor.cpp
impl/autochess/state_StopCoor_impl.cpp

autogen/autochess/state_EnterMainUI.cpp
impl/autochess/state_EnterMainUI_impl.cpp

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
