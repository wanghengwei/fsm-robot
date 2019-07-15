
#include "state_CEventChooseRole.h"
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventChooseRole.h>
#include <share/server_instance/server_address_info.h>
#include <modules/channel/shared/events/EventJoinChannelResult.h>
#include <modules/login/shared/events/EventChooseRoleFail.h>
#include <robot/basic_robot.h>
#include <robot/connection_types.h>

namespace state {

    void StateCEventChooseRole::perform(std::map<std::string, std::string>& info) {
        // // todo
        // throw std::runtime_error{"todo"};
        TPersistID roleId;
        bool ok = testcase().getData("roleId", roleId);
        if (!ok) {
            loggers::TESTCASE().error("[{}] {} FAILED: no playerdata roleId", this->testcase().id(), this->label());
            return;
        }

        CEventChooseRole ev{roleId, 0};
        auto& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
            auto e = static_cast<IEvent*>(ve);
            if (e->GetCLSID() == CLSID_CEventJoinChannelResult) {
                auto ev = static_cast<CEventJoinChannelResult*>(e);
                if (ev->GetResult() == 1) {
                    loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                    Q_EMIT this->ev_CEventJoinChannelResult_ok();
                } else {
                    loggers::TESTCASE().error("[{}] {} FAILED: ec={}", this->testcase().id(), this->label(), ev->GetResult());
                    Q_EMIT this->ev_CEventJoinChannelResult_failed();
                }
            } else if (e->GetCLSID() == CLSID_CEventChooseRoleFail) {
                loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                Q_EMIT this->ev_CEventChooseRoleFail();
            }
        });
        // loggers::TESTCASE().info("[{}] {} 11111", this->testcase().id(), this->label());
        conn.sendEvent(&ev);
	}
	
	void StateCEventChooseRole::clean() {
		auto& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
	}

}
