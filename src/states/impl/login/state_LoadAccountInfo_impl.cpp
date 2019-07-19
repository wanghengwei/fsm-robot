
#include <login/state_LoadAccountInfo.h>
#include <stdexcept>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
#include <modules/login/shared/events/EventClientLoadAccountInfo.h>
#include <modules/playermanager/shared/events/EventAccountInfo.h>

namespace state {
namespace login {
    class StateLoadAccountInfoImpl final : public StateLoadAccountInfo {
    public:
        using StateLoadAccountInfo::StateLoadAccountInfo;

        void perform() override {
            // todo
            // throw std::runtime_error{"todo"};
            CEventClientLoadAccountInfo ev;
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
                IEvent* e = static_cast<IEvent*>(ve);
                if (e->GetCLSID() == CLSID_CEventAccountInfo) {
                    auto ev = static_cast<CEventAccountInfo*>(e);
                    int rc = ev->m_info.GetRoleCount();
                    if (rc == 0) {
                        // loggers::TESTCASE().error("[{}] {} FAILED: no role", this->testcase().id(), this->label());
                        writeEndLogFailed("no role");
                        Q_EMIT this->ev_no_role();
                        return;
                    }

                    auto rid = ev->m_info.GetCharInfoByPos(0)->pstid;
                    auto sex = ev->m_info.GetCharInfoByPos(0)->sex;
                    std::string nick = ev->m_info.GetCharInfoByPos(0)->nick;
                    testcase().insertOrUpdateData("roleId", rid);
                    testcase().insertOrUpdateData("sex", sex);
                    testcase().insertOrUpdateData("nickname", nick);
                    // loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                    writeEndLogOK();
                    Q_EMIT this->ev_ok();
                }
            });
            writeBeginLog();
            conn.sendEvent(&ev);
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }
    };

    StateLoadAccountInfo* StateLoadAccountInfo::create(QState* parent) {
        return new StateLoadAccountInfoImpl{parent};
    }
}
}