
#include "state_DisconnectGame.h"

namespace state {
namespace login {

    StateDisconnectGame::StateDisconnectGame(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

}
}
