
#include <autochess/state_StartCoor.h>
#include <logger.h>
#include <net_base/basic_robot.h>
#include <net_x51/connection_types.h>
#include <modules/auto_chess/share/event/auto_chess_event.h>
#include <net_x51/utils.h>

namespace state {
namespace autochess {

	class StateStartCoorImpl final : public StateStartCoor {
	public:
		using StateStartCoor::StateStartCoor;
		
		void perform() override {
			CEventAutoChessStartCoor ev;
			GET_CONFIG_OR_DIE("/AutoChess/start_coor/type", ev.m_type);
			ev.m_is_client = true;

			auto& conn = robot().connection(CONN_GAME);
			QObject::connect(&conn, &BasicConnection::eventReceived, this, [this](void* ve) {
				if (auto ev = eventCast<CEventAutoChessStartCoorRes>(ve)) {
					if (ev->m_result != 0) {
						writeEndLogFailed(ev->m_result);
						Q_EMIT this->ev_failed();
						return;
					}

					writeEndLogOK();
					Q_EMIT this->ev_ok();
				}
			});

			writeBeginLog({
				{"type", ev.m_type},
			});
			conn.sendEvent(&ev);
		}
		
		void clean() override {
			// todo

			// // 取消注册的回调
			auto& conn = robot().connection(CONN_GAME);
			conn.disconnect(this);
		}
	};

	StateStartCoor* StateStartCoor::create(QState* parent) {
		return new StateStartCoorImpl{parent};
	}
}
}
