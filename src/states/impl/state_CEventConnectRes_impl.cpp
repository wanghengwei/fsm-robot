
#include "state_CEventConnectRes.h"
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventClientConnectRes.h>
#include <robot/connection_types.h>
#include <robot/basic_robot.h>

namespace state {

    void StateCEventConnectRes::perform(std::map<std::string, std::string>& info) {
        auto& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::eventReceived, this, [=](void* ve) {
            IEvent* e = static_cast<IEvent*>(ve);
            if (e->GetCLSID() == CLSID_CEventConnectRes) {
                auto ev = static_cast<CEventConnectRes*>(e);
                if (ev->GetLoginRes() == 0) {
                    loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                    Q_EMIT this->ev_CEventConnectRes_ok();
                } else {
                    loggers::TESTCASE().error("[{}] {} FAILED: ec={}", this->testcase().id(), this->label(), ev->GetLoginRes());
                    Q_EMIT this->ev_CEventConnectRes_failed();
                }
            }
        });
	}
	
	void StateCEventConnectRes::clean() {
		auto& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
	}

}
