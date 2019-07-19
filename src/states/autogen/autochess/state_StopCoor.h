
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace autochess {

// 状态描述：
class StateStopCoor : public BasicState {
    Q_OBJECT
public:
	static StateStopCoor* create(QState* parent);
    explicit StateStopCoor(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
};

}
}
