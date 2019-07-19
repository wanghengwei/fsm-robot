
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace autochess {

// 状态描述：
class StateStartCoor : public BasicState {
    Q_OBJECT
public:
	static StateStartCoor* create(QState* parent);
    explicit StateStartCoor(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
}
