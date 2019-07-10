
#include "state_SleepInRoom.h"

namespace state {
    StateSleepInRoom::StateSleepInRoom(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        
    }

	bool StateSleepInRoom::printLog() const { return false; }
}
