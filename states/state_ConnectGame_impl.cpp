#include "state_ConnectGame.h"
#include <QtCore/QDebug>

namespace state {

    void StateConnectGame::perform() {
        // todo
        qDebug() << "connect game";
        QTimer::singleShot(1000, this, &StateConnectGame::ev_ok);
    }

}