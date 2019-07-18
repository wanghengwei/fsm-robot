
#include <autochess/state_CEventAutoChessNotifyClientCreateMatchRes.h>
#include <logger.h>

namespace state {

	class StateCEventAutoChessNotifyClientCreateMatchResImpl final : public StateCEventAutoChessNotifyClientCreateMatchRes {
	public:
		using StateCEventAutoChessNotifyClientCreateMatchRes::StateCEventAutoChessNotifyClientCreateMatchRes;
		
		void perform() override {
			// todo
		}
		
		void clean() override {
			// todo
		}
	};

	StateCEventAutoChessNotifyClientCreateMatchRes* StateCEventAutoChessNotifyClientCreateMatchRes::create(QState* parent) {
		return new StateCEventAutoChessNotifyClientCreateMatchResImpl{parent};
	}
}
