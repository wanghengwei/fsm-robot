#include "state_factory.h"
#include "idle.h"

#include "states/state_ConnectGame.h"

#include "states/state_DisconnectGame.h"

#include "states/state_CEventLogin.h"

#include "states/state_CEventHeart.h"

#include "states/state_InRoom.h"


QState* createStateByID(QString id, QState* parent) {
    if (id == "Idle") {
        return new Idle{parent};
    }

    
    if (id == "ConnectGame") {
        return new state::StateConnectGame{parent};
    }
    
    if (id == "DisconnectGame") {
        return new state::StateDisconnectGame{parent};
    }
    
    if (id == "CEventLogin") {
        return new state::StateCEventLogin{parent};
    }
    
    if (id == "CEventHeart") {
        return new state::StateCEventHeart{parent};
    }
    
    if (id == "InRoom") {
        return new state::StateInRoom{parent};
    }
    

    return nullptr;
}