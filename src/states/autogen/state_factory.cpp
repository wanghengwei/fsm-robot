
#include <testcase/idle.h>

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
        return state::StateConnectGame::create(parent);
    }
    
    if (id == "DisconnectGame") {
        return state::StateDisconnectGame::create(parent);
    }
    
    if (id == "GameConnected") {
        return state::StateGameConnected::create(parent);
    }
    
    if (id == "CEventConnectRes") {
        return state::StateCEventConnectRes::create(parent);
    }
    
    if (id == "CEventRefreshPlayerInfo") {
        return state::StateCEventRefreshPlayerInfo::create(parent);
    }
    
    if (id == "CEventClientLoadAccountInfo") {
        return state::StateCEventClientLoadAccountInfo::create(parent);
    }
    
    if (id == "CEventEditRole") {
        return state::StateCEventEditRole::create(parent);
    }
    
    if (id == "CEventChooseRole") {
        return state::StateCEventChooseRole::create(parent);
    }
    

    return nullptr;
}
