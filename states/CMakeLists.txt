set(SRCS

state_ConnectGame.cpp

state_ConnectGame_impl.cpp

state_DisconnectGame.cpp

state_DisconnectGame_impl.cpp

state_CEventLogin.cpp

state_CEventLogin_impl.cpp

state_CEventHeart.cpp

state_CEventHeart_impl.cpp

state_InRoom.cpp

state_InRoom_impl.cpp

)

add_library(states ${SRCS})
target_link_libraries(states Qt5::Core)