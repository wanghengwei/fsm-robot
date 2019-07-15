
#include "state_factory.h"
#include "states/idle.h"

#include "states/autogen/state_ConnectGame.h"

#include "states/autogen/state_DisconnectGame.h"

#include "states/autogen/state_GameConnected.h"

#include "states/autogen/state_CEventConnectRes.h"

#include "states/autogen/state_CEventRefreshPlayerInfo.h"

#include "states/autogen/state_CEventClientLoadAccountInfo.h"

#include "states/autogen/state_CEventEditRole.h"

#include "states/autogen/state_CEventChooseRole.h"


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
    
    if (id == "GameConnected") {
        return new state::StateGameConnected{parent};
    }
    
    if (id == "CEventConnectRes") {
        return new state::StateCEventConnectRes{parent};
    }
    
    if (id == "CEventRefreshPlayerInfo") {
        return new state::StateCEventRefreshPlayerInfo{parent};
    }
    
    if (id == "CEventClientLoadAccountInfo") {
        return new state::StateCEventClientLoadAccountInfo{parent};
    }
    
    if (id == "CEventEditRole") {
        return new state::StateCEventEditRole{parent};
    }
    
    if (id == "CEventChooseRole") {
        return new state::StateCEventChooseRole{parent};
    }
    

    return nullptr;
}
