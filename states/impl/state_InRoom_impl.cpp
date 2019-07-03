
#include "state_InRoom.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

namespace state {

    void StateInRoom::perform() {
        // todo
        // throw std::runtime_error{"todo"};
        qDebug() << "in room!";
        QTimer::singleShot(10000, this, [=]() {
            emit this->ev_leaveRoom();
        });
    }

}
