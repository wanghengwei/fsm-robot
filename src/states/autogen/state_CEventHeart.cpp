
#include "state_CEventHeart.h"

namespace state {
    StateCEventHeart::StateCEventHeart(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

	bool StateCEventHeart::printLog() const { return false; }
}
