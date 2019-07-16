
#include "state_GameConnected.h"
#include <stdexcept>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>

namespace state {

    class StateGameConnectedImpl final : public StateGameConnected {
    public:
        using StateGameConnected::StateGameConnected;

        void perform() override {
            // todo
            // throw std::runtime_error{"todo"};
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::kicked, this, [this]() {
                loggers::TESTCASE().warn("[{}] be kicked from game server", this->testcase().id());
                Q_EMIT this->ev_kicked();
            });
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }
    };

    StateGameConnected* StateGameConnected::create(QState* parent) {
        return new StateGameConnectedImpl{parent};
    }
}
