
#include <login/state_WaitConnectRes.h>
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventClientConnectRes.h>
#include <net_x51/connection_types.h>
#include <net_base/basic_robot.h>

namespace state {
namespace login {
    class StateWaitConnectResImpl final : public StateWaitConnectRes {
    public:
        using StateWaitConnectRes::StateWaitConnectRes;

        void perform() override{
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::eventReceived, this, [=](void* ve) {
                IEvent* e = static_cast<IEvent*>(ve);
                if (e->GetCLSID() == CLSID_CEventConnectRes) {
                    auto ev = static_cast<CEventConnectRes*>(e);
                    if (ev->GetLoginRes() == 0) {
                        // loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                        writeEndLogOK();
                        Q_EMIT this->ev_ok();
                    } else {
                        writeEndLogFailed(ev->GetLoginRes());
                        // loggers::TESTCASE().error("[{}] {} FAILED: ec={}", this->testcase().id(), this->label(), ev->GetLoginRes());
                        Q_EMIT this->ev_failed();
                    }
                }
            });
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }
    };

    StateWaitConnectRes* StateWaitConnectRes::create(QState* parent) {
        return new StateWaitConnectResImpl{parent};
    }
}
}