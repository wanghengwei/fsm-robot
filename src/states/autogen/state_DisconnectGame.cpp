
#include "state_DisconnectGame.h"

namespace state {
    StateDisconnectGame::StateDisconnectGame(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

	bool StateDisconnectGame::printLog() const { return true; }
}
