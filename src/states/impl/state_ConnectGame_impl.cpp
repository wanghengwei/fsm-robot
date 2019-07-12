
#include "state_ConnectGame.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <logger.h>
#include <QtCore/QDebug>
#include <nlohmann/json.hpp>
#include <robot/basic_robot.h>
#include <robot/basic_connection.h>
#include <robot/connection_types.h>


namespace state {

    void StateConnectGame::perform(std::map<std::string, std::string>& info) {
        // 取出关心的数据
        nlohmann::json gameips;
        GET_DATA_OR_DIE("gameServerIPList", gameips);
        if (gameips.empty()) {
            // 触发错误怎么做？
            return;
        }

        int idx = std::rand() % gameips.size();
        nlohmann::json ipport = gameips[idx];

        // 准备log信息
        info["host"] = ipport.dump();

        std::string ip = ipport["ip"];
        int port = ipport["port"];

        // 发起连接，并关注连接事件
        BasicConnection& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::connectOK, this, [=]() {
            loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
            Q_EMIT this->ev_ok();
        });
        QObject::connect(&conn, &BasicConnection::connectFailed, this, [=](const std::string& msg) {
            loggers::TESTCASE().error("[{}] {} FAILED: {}", this->testcase().id(), this->label(), msg);
            Q_EMIT this->ev_failed();
        });
        conn.connect(ip, port, id());
    }

    void StateConnectGame::clean() {
        // 清理关注
        BasicConnection& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
        // loggers::TESTCASE().info("clean connect");
    }

}
