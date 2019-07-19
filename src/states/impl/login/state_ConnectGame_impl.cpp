
#include <login/state_ConnectGame.h>
#include <stdexcept>
#include <QtCore/QTimer>
#include <logger.h>
#include <QtCore/QDebug>
#include <nlohmann/json.hpp>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>


namespace state {
namespace login {
    class StateConnectGameImpl final : public StateConnectGame {
    public:
        using StateConnectGame::StateConnectGame;

        void perform() override {
            // 取出关心的数据
            nlohmann::json gameips;
            GET_CONFIG_OR_DIE("/gameServerIPList", gameips);
            if (gameips.empty()) {
                // 触发错误怎么做？
                return;
            }

            int idx = std::rand() % gameips.size();
            nlohmann::json ipport = gameips[idx];

            // // 准备log信息
            // info["host"] = ipport.dump();

            std::string ip = ipport["ip"];
            int port = ipport["port"];

            // 发起连接，并关注连接事件
            BasicConnection& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::connectOK, this, [=]() {
                writeEndLogOK();
                Q_EMIT this->ev_ok();
            });
            QObject::connect(&conn, &BasicConnection::connectFailed, this, [=](const std::string& msg) {
                // loggers::TESTCASE().error("[{}] {} FAILED: {}", this->testcase().id(), this->label(), msg);
                writeEndLogFailed(msg);
                Q_EMIT this->ev_failed();
            });

            writeBeginLog({{"ip", ip}, {"port", port}});
            conn.connect(ip, port, id());
        }

        void clean() override {
            // 清理关注
            BasicConnection& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
            // loggers::TESTCASE().info("clean connect");
        }
    };

    StateConnectGame* StateConnectGame::create(QState* parent) {
        return new StateConnectGameImpl{parent};
    }
}
}