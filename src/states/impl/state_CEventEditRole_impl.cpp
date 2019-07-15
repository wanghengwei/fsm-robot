
#include "state_CEventEditRole.h"
#include <stdexcept>
#include <logger.h>
#include <modules/login/shared/events/EventEditRole.h>
#include <modules/login/shared/events/EventEditRoleFail.h>
#include <modules/playermanager/shared/events/EventAccountInfo.h>
#include <robot/connection_types.h>
#include <robot/basic_robot.h>
#include <utils.h>

namespace state {

    void StateCEventEditRole::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        char_create_info ccinfo;
        // const char* nick = robot.account.toUtf8().data();
        ccinfo.sex = QString::fromStdString(testcase().id()).toLongLong() % 2;
        ccinfo.birthday.set_time((time32::time(0) - time32(3600 * 24 * 365 * 20)).value());
        std::string nick;
        bool ok = testcase().getData("nickname", nick);
        if (!ok) {
            nick = testcase().id();
        }
        snprintf(ccinfo.nick, MaxCharacterNamelen + 1, "%s", nick.data());

        ChoosedAvatar avatar;
        nlohmann::json t;
        ok = testcase().getData("avatar_info", t);
        if (!ok) {
            // todo
            // emit error!
            loggers::TESTCASE().error("no playerdata avatar_info");
            return;
        }

        nlohmann::json avatarConfig;
        if (ccinfo.sex == 0) {
            avatarConfig = t["female"];
        } else {
            avatarConfig = t["male"];
        }
        utils::random_get_to(avatarConfig["skin"], avatar.m_chooseavatar[AAI_Skin]);
        utils::random_get_to(avatarConfig["hair"], avatar.m_chooseavatar[AAI_Hair]);
        utils::random_get_to(avatarConfig["eyes"], avatar.m_chooseavatar[AAI_Eyes]);
        auto c = utils::sample(avatarConfig["cloth"]);
        c["face"].get_to(avatar.m_chooseavatar[AAI_Face]);
        c["upper"].get_to(avatar.m_chooseavatar[AAI_UpperClothing]);
        c["under"].get_to(avatar.m_chooseavatar[AAI_UnderClothing]);
        c["gloves"].get_to(avatar.m_chooseavatar[AAI_Gloves]);
        c["socks"].get_to(avatar.m_chooseavatar[AAI_Socks]);
        c["shoes"].get_to(avatar.m_chooseavatar[AAI_Shoes]);

        CEventEditRole ev{ccinfo, avatar};

        auto& conn = robot().connection(CONN_GAME);
        QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
            IEvent* e = static_cast<IEvent*>(ve);
            if (e->GetCLSID() == CLSID_CEventAccountInfo) {
                auto ev = static_cast<CEventAccountInfo*>(e);
                auto rid = ev->m_info.GetCharInfoByPos(0)->pstid;
                auto sex = ev->m_info.GetCharInfoByPos(0)->sex;
                std::string nick = ev->m_info.GetCharInfoByPos(0)->nick;
                testcase().insertOrUpdateData("roleId", rid);
                testcase().insertOrUpdateData("sex", sex);
                testcase().insertOrUpdateData("nickname", nick);
                loggers::TESTCASE().info("[{}] {} OK", this->testcase().id(), this->label());
                Q_EMIT this->ev_CEventAccountInfo();
            } else if (e->GetCLSID() == CLSID_CEventEditRoleRes) {
                auto ev = static_cast<CEventEditRoleRes*>(e);
                if (ev->m_reason == 2) {
                    std::string rn = ev->m_nickname_recommend;
                    testcase().insertOrUpdateData("m_nickname_recommend", rn);
                    loggers::TESTCASE().info("[{}] {} FAILED: nick error", this->testcase().id(), this->label());
                    Q_EMIT this->ev_CEventEditRoleRes_nick();
                } else {
                    loggers::TESTCASE().info("[{}] {} FAILED: ec={}", this->testcase().id(), this->label(), ev->m_reason);
                    Q_EMIT this->ev_CEventEditRoleRes_failed();
                }
            }
        });
        conn.sendEvent(&ev);
	}
	
	void StateCEventEditRole::clean() {
		auto& conn = robot().connection(CONN_GAME);
        conn.disconnect(this);
	}

}
