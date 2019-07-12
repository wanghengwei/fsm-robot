
#include "state_ConnectGame.h"

namespace state {
    StateConnectGame::StateConnectGame(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

	bool StateConnectGame::printLog() const { return true; }
}
