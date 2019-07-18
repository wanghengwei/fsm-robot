
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：
class StateCEventAutoChessStartCoor : public BasicState {
    Q_OBJECT
public:
	static StateCEventAutoChessStartCoor* create(QState* parent);
    explicit StateCEventAutoChessStartCoor(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
