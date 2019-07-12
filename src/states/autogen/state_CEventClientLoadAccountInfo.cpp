
#include "state_CEventClientLoadAccountInfo.h"

namespace state {
    StateCEventClientLoadAccountInfo::StateCEventClientLoadAccountInfo(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }

	bool StateCEventClientLoadAccountInfo::printLog() const { return false; }
}
