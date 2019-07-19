
#include "state_WaitCoorResult.h"

namespace state {
namespace autochess {

    StateWaitCoorResult::StateWaitCoorResult(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, [this]() {
			writeEndLogFailed("timeout");
			Q_EMIT this->ev_timeout();
		});
        
    }

}
}
