
#include <testcase/idle.h>

#include "login/state_ConnectGame.h"

#include "login/state_DisconnectGame.h"

#include "login/state_GameConnected.h"

#include "login/state_WaitConnectRes.h"

#include "login/state_RefreshPlayerInfo.h"

#include "login/state_LoadAccountInfo.h"

#include "login/state_EditRole.h"

#include "login/state_ChooseRole.h"

#include "login/state_Logined.h"

#include "autochess/state_StartCoor.h"

#include "autochess/state_WaitCoorResult.h"

#include "autochess/state_NotifyClientCreateMatchRes.h"

#include "autochess/state_StopCoor.h"

#include "autochess/state_EnterMainUI.h"


QState* createStateByID(QString id, QState* parent) {
    if (id == "Idle") {
        return new Idle{parent};
    }

    
    if (id == "ConnectGame") {
        return state::login::StateConnectGame::create(parent);
    }
    
    if (id == "DisconnectGame") {
        return state::login::StateDisconnectGame::create(parent);
    }
    
    if (id == "GameConnected") {
        return state::login::StateGameConnected::create(parent);
    }
    
    if (id == "WaitConnectRes") {
        return state::login::StateWaitConnectRes::create(parent);
    }
    
    if (id == "RefreshPlayerInfo") {
        return state::login::StateRefreshPlayerInfo::create(parent);
    }
    
    if (id == "LoadAccountInfo") {
        return state::login::StateLoadAccountInfo::create(parent);
    }
    
    if (id == "EditRole") {
        return state::login::StateEditRole::create(parent);
    }
    
    if (id == "ChooseRole") {
        return state::login::StateChooseRole::create(parent);
    }
    
    if (id == "Logined") {
        return state::login::StateLogined::create(parent);
    }
    
    if (id == "StartCoor") {
        return state::autochess::StateStartCoor::create(parent);
    }
    
    if (id == "WaitCoorResult") {
        return state::autochess::StateWaitCoorResult::create(parent);
    }
    
    if (id == "NotifyClientCreateMatchRes") {
        return state::autochess::StateNotifyClientCreateMatchRes::create(parent);
    }
    
    if (id == "StopCoor") {
        return state::autochess::StateStopCoor::create(parent);
    }
    
    if (id == "EnterMainUI") {
        return state::autochess::StateEnterMainUI::create(parent);
    }
    

    return nullptr;
}
