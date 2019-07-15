
#include "state_GameConnected.h"
#include <stdexcept>
#include <logger.h>
#include <robot/basic_robot.h>
#include <robot/connection_types.h>

namespace state {

    void StateGameConnected::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        auto& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::kicked, this, [this]() {
            loggers::TESTCASE().warn("[{}] be kicked from game server", this->testcase().id());
            Q_EMIT this->ev_kicked();
        });
	}
	
	void StateGameConnected::clean() {
		auto& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
	}

}
