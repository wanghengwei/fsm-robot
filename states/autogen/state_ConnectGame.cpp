
#include "state_ConnectGame.h"

namespace state {
    StateConnectGame::StateConnectGame(QState* parent) : BasicState{parent} {
        
        setTimeout(60000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }
}
