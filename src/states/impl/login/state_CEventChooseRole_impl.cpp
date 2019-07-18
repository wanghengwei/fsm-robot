
#include <login/state_CEventChooseRole.h>
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

    class StateCEventChooseRoleImpl final : public StateCEventChooseRole {
    public:
        using StateCEventChooseRole::StateCEventChooseRole;

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
                        Q_EMIT this->ev_CEventJoinChannelResult_ok();
                    } else {
                        writeEndLogFailed(ev->GetResult());
                        Q_EMIT this->ev_CEventJoinChannelResult_failed();
                    }
                } else if (auto ev = eventCast<CEventChooseRoleFail>(ve)) {
                    writeEndLogFailed("");
                    Q_EMIT this->ev_CEventChooseRoleFail();
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

    StateCEventChooseRole* StateCEventChooseRole::create(QState* parent) {
        return new StateCEventChooseRoleImpl{parent};
    }

}
