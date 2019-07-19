
#include <login/state_Logined.h>
#include <logger.h>

namespace state {
namespace login {
	class StateLoginedImpl final : public StateLogined {
	public:
		using StateLogined::StateLogined;
		
		void perform() override {
			loggers::TESTCASE().info("[{}] Logined", this->testcase().id(), this->label());
		}
		
		void clean() override {
			loggers::TESTCASE().info("[{}] Logout", this->testcase().id(), this->label());
		}
	};

	StateLogined* StateLogined::create(QState* parent) {
		return new StateLoginedImpl{parent};
	}
}
}