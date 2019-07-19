
#include <autochess/state_CEventAutoChessEnterMainUI.h>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
#include <modules/auto_chess/share/event/auto_chess_event.h>
#include <net_x51/utils.h>

namespace state {

	class StateCEventAutoChessEnterMainUIImpl final : public StateCEventAutoChessEnterMainUI {
	public:
		using StateCEventAutoChessEnterMainUI::StateCEventAutoChessEnterMainUI;
		
		void perform() override {
			// todo

			// // 收发消息的范例
			// 
			// // 准备发送的消息
			CEventAutoChessEnterMainUI ev;
			auto& conn = robot().connection(CONN_GAME);
			// 实现收到消息后的回调
			QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
				if (auto rev = eventCast<CEventAutoChessEnterMainUIRes>(ve)) {
					testcase().insertOrUpdateData("reconnect_info_state", rev->m_reconnect_info.m_state);
					testcase().insertOrUpdateData("auto_chess_rand_pwd", rev->m_reconnect_info.m_room_info.m_rand_password);
					testcase().insertOrUpdateData("auto_chess_room_id", rev->m_reconnect_info.m_room_info.m_room_id);
					testcase().insertOrUpdateData("match_player_id", rev->m_reconnect_info.m_match_player_id);
					testcase().insertOrUpdateData("auto_chess_coor_players", rev->m_reconnect_info.m_room_players);
					testcase().insertOrUpdateData("matchServiceHost", QString(rev->m_reconnect_info.m_room_info.m_addr.GetAddress(0).m_ip));
					testcase().insertOrUpdateData("matchServicePort", rev->m_reconnect_info.m_room_info.m_addr.GetAddress(0).m_port);

					if (rev->m_result != 0) {
						writeEndLogFailed(rev->m_result);
						Q_EMIT this->ev_failed();
						return;
					}

					if (rev->m_reconnect_info.m_state == EACRS_Idle) {
						writeEndLogOK();
						Q_EMIT this->ev_ok();
					} else if (rev->m_reconnect_info.m_state == EACRS_Coordinated) {
						writeEndLogOK();
						Q_EMIT this->ev_reconnect_coordinated();
					} else if (rev->m_reconnect_info.m_state == EACRS_InGame) {
						writeEndLogOK();
						Q_EMIT this->ev_reconnect_ingame();
					} else {
						// todo ASSERT
					}
					
				}
			});

			writeBeginLog();
			// 发送消息
			conn.sendEvent(&ev);
		}
		
		void clean() override {
			// todo

			// // 取消注册的回调
			auto& conn = robot().connection(CONN_GAME);
			conn.disconnect(this);
		}
	};

	StateCEventAutoChessEnterMainUI* StateCEventAutoChessEnterMainUI::create(QState* parent) {
		return new StateCEventAutoChessEnterMainUIImpl{parent};
	}
}
