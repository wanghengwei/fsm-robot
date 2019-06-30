#include "state_CEventLogin.h"

namespace state {
    StateCEventLogin::StateCEventLogin(QState* parent) : BaseState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BaseState::ev_timeout);
        
    }
}