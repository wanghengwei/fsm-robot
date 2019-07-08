
#include "state_InRoom.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <logger.h>

namespace state {

    void StateInRoom::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        // qDebug() << "in room!";
        loggers::TESTCASE().info("[{}] in room", testcase().id());
        QTimer::singleShot(10000, Qt::VeryCoarseTimer, this, [=]() {
            emit this->ev_leaveRoom();
        });
    }

    void StateInRoom::clean() {}
}
