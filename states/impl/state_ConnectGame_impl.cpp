
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <QtCore/QRandomGenerator>
#include <logger.h>
#include <QtCore/QDebug>

namespace state {

    void StateConnectGame::perform() {
        QList<QString> gameips;
        GET_DATA_OR_DIR("state.ConnectGame.iplist", gameips);
        int idx = QRandomGenerator::global()->bounded(gameips.size() - 1);
        QString ip = gameips[idx];
        // robot()->connection("game").connect(ip);
        // QObject::connect(robot(), &Robot::connect_ok, this, [=](const QString& n) {
        //     if (n == "game") {
        //         emit this->ev_ok();
        //     }
        // });

        QTimer::singleShot(1000, this, [=]() {
            emit this->ev_ok();
        });
    }

}
