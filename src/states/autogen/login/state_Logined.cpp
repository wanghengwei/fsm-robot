
#include "state_Logined.h"

namespace state {
namespace login {

    StateLogined::StateLogined(QState* parent) : BasicState{parent} {
        
        setTimeout(-1);
        
    }

}
}
