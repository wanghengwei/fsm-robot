
#include "state_CEventConnectRes.h"

namespace state {
    StateCEventConnectRes::StateCEventConnectRes(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }

	bool StateCEventConnectRes::printLog() const { return true; }
}
