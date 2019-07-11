
#include "state_CEventLogin.h"

namespace state {
    StateCEventLogin::StateCEventLogin(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }

	bool StateCEventLogin::printLog() const { return true; }
}
