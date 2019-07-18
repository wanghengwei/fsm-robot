
#include <login/state_CEventRefreshPlayerInfo.h>
#include <stdexcept>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
#include <modules/playermanager/shared/events/EventRefreshPlayerInfo.h>
#include <modules/playermanager/shared/events/EventInitializePlayerInfo.h>

namespace state {

    namespace {
        void refreshPlayerInfo(const character_info &info, TestCase& c) {

            c.insertOrUpdateData("sex", info.sex);
            c.insertOrUpdateData("nickname", QString{info.nick});

            std::vector<int> pkg_type{ 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008 };
            c.insertOrUpdateData("player_pkg_type", pkg_type);

            std::map<int, qlonglong> player_package_infos;
            player_package_infos[PKG_Appearance] = info.m_appearpkg_id.id;
            player_package_infos[PKG_Equip] = info.m_equippkg_id.id;
            player_package_infos[PKG_Medal] = info.m_medalpkg_id.id;
            player_package_infos[PKG_RoomFitment] = info.m_roomfitmentpkg_id.id;
            player_package_infos[PKG_Pantechnicon] = info.m_pantechniconpkg_id.id;
            player_package_infos[PKG_Hidden] = info.m_hiddenpkg_id.id;
            player_package_infos[PKG_RecycledFitments] = info.m_recycledfitmentspkg_id.id;
            player_package_infos[PKG_Appearance2] = info.m_appearpkg2_id.id;
            player_package_infos[PKG_LuckyCabinet] = info.m_luckycabinetpkg_id.id;
            c.insertOrUpdateData("player_packages", player_package_infos);

            std::vector<int> player_avatar_infos(AppearItemNum, 0);
            memcpy(&(player_avatar_infos[0]), info.appearance.avatar, AppearItemNum * sizeof(int));
            c.insertOrUpdateData("player_avatar", player_avatar_infos);

            std::vector<int> player_medal_infos(AppPkgTotalMedalPosNum, 0);
            memcpy(&(player_medal_infos[0]), info.appearance.medal, AppPkgTotalMedalPosNum * sizeof(int));
            c.insertOrUpdateData("player_medal", player_medal_infos);

            // 道具信息初始化一下
            std::map<int, std::vector<qlonglong>> player_item_infos;
            // auto ec = robot.getPlayerData("player_items", player_item_infos);
            // if (ec) {
            c.insertData("player_items", player_item_infos);
            // }
        }
    }

    class StateCEventRefreshPlayerInfoImpl final : public StateCEventRefreshPlayerInfo {
    public:
        using StateCEventRefreshPlayerInfo::StateCEventRefreshPlayerInfo;

        void perform() override {
            auto& conn = robot().connection(CONN_GAME);
            QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
                IEvent* e = static_cast<IEvent*>(ve);
                if (e->GetCLSID() == CLSID_CEventRefreshPlayerInfo) {
                    auto ev = static_cast<CEventRefreshPlayerInfo*>(e);
                    refreshPlayerInfo(ev->GetPlayerInfo(), this->testcase());
                    Q_EMIT this->ev_ok();
                } else if (e->GetCLSID() == CLSID_CEventInitializePlayerInfo) {
                    auto ev = static_cast<CEventInitializePlayerInfo*>(e);
                    testcase().insertOrUpdateData("zoneId", ev->GetZoneID());
                    testcase().insertOrUpdateData("pstid", ev->GetPlayerInfo().pstid.id);
                    refreshPlayerInfo(ev->GetPlayerInfo(), this->testcase());
                    Q_EMIT this->ev_ok();
                }
            });
        }
        
        void clean() override {
            auto& conn = robot().connection(CONN_GAME);
            conn.disconnect(this);
        }
    };

    StateCEventRefreshPlayerInfo* StateCEventRefreshPlayerInfo::create(QState* parent) {
        return new StateCEventRefreshPlayerInfoImpl{parent};
    }
}
