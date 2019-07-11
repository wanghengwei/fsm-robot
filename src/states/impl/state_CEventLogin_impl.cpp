
#include "state_CEventLogin.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <logger.h>
#include <robot/basic_robot.h>
#include <connection_types.h>

namespace state {

    void StateCEventLogin::perform(std::map<std::string, std::string>& info) {
        int zone = 0;
        GET_DATA_OR_DIE("zone", zone);
        info["zone"] = std::to_string(zone);

        auto& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::eventReceived, this, [=, &conn](void* e) {
            // conn.disconnect(this);
            if (e != nullptr) {
                int* ep = static_cast<int*>(e);
                loggers::TESTCASE().info("received event: {}", *ep);
                delete ep;
                emit this->ev_CEventLoginRes_ok();
            } else {
                loggers::TESTCASE().info("[{}] {} FAILED", this->testcase().id(), this->label());
                emit this->ev_CEventLoginRes_failed();
            }
        });
        conn.sendEvent(new int{1});
    }

    void StateCEventLogin::clean() {
        auto& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
    }

}
