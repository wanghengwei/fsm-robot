
#include "state_CEventEditRole.h"

namespace state {
    StateCEventEditRole::StateCEventEditRole(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }

	bool StateCEventEditRole::printLog() const { return true; }
}
