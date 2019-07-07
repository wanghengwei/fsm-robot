
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <QtCore/QRandomGenerator>
#include <logger.h>
#include <QtCore/QDebug>
#include <nlohmann/json.hpp>
#include <robot/basic_robot.h>
#include <robot/basic_connection.h>
#include <connection_types.h>

namespace state {

    void StateConnectGame::perform(std::map<std::string, std::string>& info) {
        nlohmann::json gameips;
        GET_DATA_OR_DIE("gameServerIPList", gameips);
        int idx = QRandomGenerator::global()->bounded(int(gameips.size()) - 1);
        std::string ip = gameips[idx];

        info["ip"] = ip;

        QObject::connect(&robot(), &BasicRobot::connectOK, this, &StateConnectGame::ev_ok);
        robot().connection(CONN_GAME).connect(id(), ip);
        // loggers::TESTCASE().debug("connect game {}", ip);
        // robot()->connection("game").connect(ip);
        // QObject::connect(robot(), &Robot::connect_ok, this, [=](const QString& n) {
        //     if (n == "game") {
        //         emit this->ev_ok();
        //     }
        // });

        // QTimer::singleShot(1000, this, [=]() {
        //     emit this->ev_ok();
        // });
    }

}
