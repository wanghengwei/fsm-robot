
#include "state_Logined.h"
#include <logger.h>

namespace state {

	class StateLoginedImpl final : public StateLogined {
	public:
		using StateLogined::StateLogined;
		
		void perform() override {
			// todo
		}
		
		void clean() override {
			// todo
		}
	};

	StateLogined* StateLogined::create(QState* parent) {
		return new StateLoginedImpl{parent};
	}
}
