
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
        QObject::connect(&conn, &BasicConnection::eventReceived, this, [=](void* e) {
            if (!e) {
                emit this->ev_CEventLoginRes_ok();
            } else {
                emit this->ev_CEventLoginRes_failed();
            }
        });
        conn.sendEvent(nullptr);
    }

}
