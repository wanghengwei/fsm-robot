
#include "state_CEventConnectRes.h"
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventClientConnectRes.h>
#include <net_x51/connection_types.h>
#include <net_base/basic_robot.h>

namespace state {
    class StateCEventConnectResImpl final : public StateCEventConnectRes {
    public:
        using StateCEventConnectRes::StateCEventConnectRes;

        void perform() override{
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::eventReceived, this, [=](void* ve) {
                IEvent* e = static_cast<IEvent*>(ve);
                if (e->GetCLSID() == CLSID_CEventConnectRes) {
                    auto ev = static_cast<CEventConnectRes*>(e);
                    if (ev->GetLoginRes() == 0) {
                        // loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                        writeEndLogOK();
                        Q_EMIT this->ev_CEventConnectRes_ok();
                    } else {
                        writeEndLogFailed(ev->GetLoginRes());
                        // loggers::TESTCASE().error("[{}] {} FAILED: ec={}", this->testcase().id(), this->label(), ev->GetLoginRes());
                        Q_EMIT this->ev_CEventConnectRes_failed();
                    }
                }
            });
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }
    };

    StateCEventConnectRes* StateCEventConnectRes::create(QState* parent) {
        return new StateCEventConnectResImpl{parent};
    }
}
