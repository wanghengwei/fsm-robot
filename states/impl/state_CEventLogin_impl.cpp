
#include "state_CEventLogin.h"
#include <stdexcept>
#include <QtCore/QTimer>

namespace state {

    void StateCEventLogin::perform() {
        // todo
        // throw std::runtime_error{"todo"};
        QTimer::singleShot(1000, this, [=]() {
            emit this->ev_CEventLoginRes_ok();
        });
    }

}
