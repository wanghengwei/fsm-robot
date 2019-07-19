
#include "state_StopCoor.h"

namespace state {
namespace autochess {

    StateStopCoor::StateStopCoor(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, [this]() {
			writeEndLogFailed("timeout");
			Q_EMIT this->ev_timeout();
		});
        
    }

}
}
