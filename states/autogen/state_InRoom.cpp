
#include "state_InRoom.h"

namespace state {
    StateInRoom::StateInRoom(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }
}
