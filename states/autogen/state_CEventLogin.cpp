
#include "state_CEventLogin.h"

namespace state {
    StateCEventLogin::StateCEventLogin(QState* parent) : BasicState{parent} {
        
        setTimeout(0);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }
}
