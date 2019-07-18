
#include <autochess/state_CEventAutoChessCoorResult.h>
#include <logger.h>

namespace state {

	class StateCEventAutoChessCoorResultImpl final : public StateCEventAutoChessCoorResult {
	public:
		using StateCEventAutoChessCoorResult::StateCEventAutoChessCoorResult;
		
		void perform() override {
			// todo
		}
		
		void clean() override {
			// todo
		}
	};

	StateCEventAutoChessCoorResult* StateCEventAutoChessCoorResult::create(QState* parent) {
		return new StateCEventAutoChessCoorResultImpl{parent};
	}
}
