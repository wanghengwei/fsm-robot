
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace autochess {

// 状态描述：
class StateWaitCoorResult : public BasicState {
    Q_OBJECT
public:
	static StateWaitCoorResult* create(QState* parent);
    explicit StateWaitCoorResult(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
}
