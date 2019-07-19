
#include "state_ChooseRole.h"

namespace state {
namespace login {

    StateChooseRole::StateChooseRole(QState* parent) : BasicState{parent} {
        
        setTimeout(10000);
        QObject::connect(this->timer(), &QTimer::timeout, this, [this]() {
			writeEndLogFailed("timeout");
			Q_EMIT this->ev_timeout();
		});
        
    }

}
}
