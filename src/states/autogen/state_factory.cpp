
#include <testcase/idle.h>

#include "login/state_ConnectGame.h"

#include "login/state_DisconnectGame.h"

#include "login/state_GameConnected.h"

#include "login/state_CEventConnectRes.h"

#include "login/state_CEventRefreshPlayerInfo.h"

#include "login/state_CEventClientLoadAccountInfo.h"

#include "login/state_CEventEditRole.h"

#include "login/state_CEventChooseRole.h"

#include "login/state_Logined.h"

#include "autochess/state_CEventAutoChessStartCoor.h"

#include "autochess/state_CEventAutoChessCoorResult.h"

#include "autochess/state_CEventAutoChessNotifyClientCreateMatchRes.h"

#include "autochess/state_CEventAutoChessStopCoor.h"

#include "autochess/state_CEventAutoChessEnterMainUI.h"


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
    
    if (id == "Logined") {
        return state::StateLogined::create(parent);
    }
    
    if (id == "CEventAutoChessStartCoor") {
        return state::StateCEventAutoChessStartCoor::create(parent);
    }
    
    if (id == "CEventAutoChessCoorResult") {
        return state::StateCEventAutoChessCoorResult::create(parent);
    }
    
    if (id == "CEventAutoChessNotifyClientCreateMatchRes") {
        return state::StateCEventAutoChessNotifyClientCreateMatchRes::create(parent);
    }
    
    if (id == "CEventAutoChessStopCoor") {
        return state::StateCEventAutoChessStopCoor::create(parent);
    }
    
    if (id == "CEventAutoChessEnterMainUI") {
        return state::StateCEventAutoChessEnterMainUI::create(parent);
    }
    

    return nullptr;
}
