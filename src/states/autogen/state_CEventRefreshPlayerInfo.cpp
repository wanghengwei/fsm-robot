
#include "state_CEventRefreshPlayerInfo.h"

namespace state {
    StateCEventRefreshPlayerInfo::StateCEventRefreshPlayerInfo(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

	bool StateCEventRefreshPlayerInfo::printLog() const { return false; }
}
