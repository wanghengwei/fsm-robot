
#include <autochess/state_CEventAutoChessStopCoor.h>
#include <logger.h>

namespace state {

	class StateCEventAutoChessStopCoorImpl final : public StateCEventAutoChessStopCoor {
	public:
		using StateCEventAutoChessStopCoor::StateCEventAutoChessStopCoor;
		
		void perform() override {
			// todo
		}
		
		void clean() override {
			// todo
		}
	};

	StateCEventAutoChessStopCoor* StateCEventAutoChessStopCoor::create(QState* parent) {
		return new StateCEventAutoChessStopCoorImpl{parent};
	}
}
