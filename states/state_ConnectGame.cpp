
#include "state_ConnectGame.h"

namespace state {
    StateConnectGame::StateConnectGame(QState* parent) : BaseState{parent} {
        
        setTimeout(60000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BaseState::ev_timeout);
        
    }
}
