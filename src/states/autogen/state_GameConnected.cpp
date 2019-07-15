
#include "state_GameConnected.h"

namespace state {
    StateGameConnected::StateGameConnected(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

	bool StateGameConnected::printLog() const { return false; }
}
