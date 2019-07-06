
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <QtCore/QRandomGenerator>

namespace state {

    void StateConnectGame::perform() {
        // todo
        // throw std::runtime_error{"todo"};
        QTimer::singleShot(1000, this, [=]() {
            emit this->ev_ok();
        });
        QList<QString> gameips;
        bool ok = this->testcase().getData("state.ConnectGame.iplist", gameips);
        if (!ok) {
            return;
        }
        int idx = QRandomGenerator::global()->bounded(gameips.size() - 1);
        QString ip = gameips[idx];
        // robot()->connection("game").connect(ip);
        // QObject::connect(robot(), &Robot::connect_ok, this, [=](const QString& n) {
        //     if (n == "game") {
        //         emit this->ev_ok();
        //     }
        // });
    }

}
