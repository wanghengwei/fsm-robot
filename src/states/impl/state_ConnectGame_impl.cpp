
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>
// #include <QtCore/QRandomGenerator>
#include <logger.h>
#include <QtCore/QDebug>
#include <nlohmann/json.hpp>
#include <robot/basic_robot.h>
#include <robot/basic_connection.h>
#include <connection_types.h>

// #define CONNECT(sig, slot) \
//     QObject::connect(&conn, &BasicConnection::sig, this, [this] () { \
//         loggers::TESTCASE().info("[{}] {}", this->testcase().id(), this->label()); \
//         emit this->ev_##slot(); \
//     })

namespace state {

    void StateConnectGame::perform(std::map<std::string, std::string>& info) {
        // 取出关心的数据
        nlohmann::json gameips;
        GET_DATA_OR_DIE("gameServerIPList", gameips);
        if (gameips.empty()) {
            // 触发错误怎么做？
            return;
        }
        // int idx = QRandomGenerator::global()->bounded(int(gameips.size()) - 1);
        int idx = std::rand() % gameips.size();
        std::string ip = gameips[idx];

        // 准备log信息
        info["ip"] = ip;

        // 发起连接，并关注连接事件
        BasicConnection& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::connectOK, this, [=]() {
            loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
            emit this->ev_ok();
        });
        QObject::connect(&conn, &BasicConnection::connectFailed, this, &StateConnectGame::ev_failed);
        conn.connect(id(), ip);
    }

    void StateConnectGame::clean() {
        // 清理关注
        BasicConnection& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
        // loggers::TESTCASE().info("clean connect");
    }

}
