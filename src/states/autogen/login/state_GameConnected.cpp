
#include "state_GameConnected.h"

namespace state {
namespace login {

    StateGameConnected::StateGameConnected(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

}
}
