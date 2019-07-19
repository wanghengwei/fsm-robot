
#include <login/state_ChooseRole.h>
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventChooseRole.h>
#include <share/server_instance/server_address_info.h>
#include <modules/channel/shared/events/EventJoinChannelResult.h>
#include <modules/login/shared/events/EventChooseRoleFail.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
#include <net_x51/utils.h>

namespace state {
namespace login {

    class StateChooseRoleImpl final : public StateChooseRole {
    public:
        using StateChooseRole::StateChooseRole;

        void perform() override {
            TPersistID roleId;
            bool ok = testcase().getData("roleId", roleId);
            if (!ok) {
                writeEndLogFailed("no playerdata roleId");
                return;
            }

            CEventChooseRole ev{roleId, 0};
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
                if (auto ev = eventCast<CEventJoinChannelResult>(ve)) {
                    if (ev->GetResult() == 1) {
                        writeEndLogOK();
                        Q_EMIT this->ev_ok();
                    } else {
                        writeEndLogFailed(ev->GetResult());
                        Q_EMIT this->ev_failed();
                    }
                } else if (auto ev = eventCast<CEventChooseRoleFail>(ve)) {
                    writeEndLogFailed("");
                    Q_EMIT this->ev_failed();
                } 
            });
            writeBeginLog({{"roleId", roleId.id}});
            conn.sendEvent(&ev);
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }

    };

    StateChooseRole* StateChooseRole::create(QState* parent) {
        return new StateChooseRoleImpl{parent};
    }

}
}