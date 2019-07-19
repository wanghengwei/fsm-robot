
#include <autochess/state_CEventAutoChessStopCoor.h>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
// #include <net_x51/utils.h>

namespace state {

	class StateCEventAutoChessStopCoorImpl final : public StateCEventAutoChessStopCoor {
	public:
		using StateCEventAutoChessStopCoor::StateCEventAutoChessStopCoor;
		
		void perform() override {
			// todo

			// // 收发消息的范例
			// 
			// // 准备发送的消息
			// CEventAutoChessStartCoor ev;
			// GET_CONFIG_OR_DIE("/AutoChess/start_coor/type", ev.m_type);
			// ev.m_is_client = true;
			// // 获得相应服务器的连接
			// auto& conn = robot().connection(CONN_GAME);
			// // 实现收到消息后的回调
			// QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
			// 	if (auto ev = eventCast<CEventAutoChessStartCoorRes>(ve)) {
			// 		if (ev->m_result != 0) {
			// 			writeEndLogFailed(ev->m_result);
			// 			Q_EMIT this->ev_failed();
			// 			return;
			// 		}
			// 		writeEndLogOK();
			// 		Q_EMIT this->ev_ok();
			// 	}
			// });

			// writeBeginLog({
			// 	{"type", ev.m_type},
			// });
			// // 发送消息
			// conn.sendEvent(&ev);
		}
		
		void clean() override {
			// todo

			// // 取消注册的回调
			// auto& conn = robot().connection(CONN_GAME);
			// conn.disconnect(this);
		}
	};

	StateCEventAutoChessStopCoor* StateCEventAutoChessStopCoor::create(QState* parent) {
		return new StateCEventAutoChessStopCoorImpl{parent};
	}
}
