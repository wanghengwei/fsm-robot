
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>

namespace state {

    void StateConnectGame::perform() {
        // todo
        // throw std::runtime_error{"todo"};
        QTimer::singleShot(1000, this, [=]() {
            emit this->ev_ok();
        });
    }

}
