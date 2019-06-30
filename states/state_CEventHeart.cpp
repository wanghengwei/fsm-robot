#include "state_CEventHeart.h"

namespace state {
    StateCEventHeart::StateCEventHeart(QState* parent) : BaseState{parent} {
        
        setTimeout(-1);
        
    }
}