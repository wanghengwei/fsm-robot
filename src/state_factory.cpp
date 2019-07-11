
#include "state_factory.h"
#include "states/idle.h"

#include "states/autogen/state_ConnectGame.h"

#include "states/autogen/state_DisconnectGame.h"

#include "states/autogen/state_CEventLogin.h"

#include "states/autogen/state_CEventHeart.h"

#include "states/autogen/state_InRoom.h"

#include "states/autogen/state_SleepInRoom.h"


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
    
    if (id == "SleepInRoom") {
        return new state::StateSleepInRoom{parent};
    }
    

    return nullptr;
}
