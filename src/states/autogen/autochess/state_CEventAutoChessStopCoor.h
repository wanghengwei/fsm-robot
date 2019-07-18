
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：
class StateCEventAutoChessStopCoor : public BasicState {
    Q_OBJECT
public:
	static StateCEventAutoChessStopCoor* create(QState* parent);
    explicit StateCEventAutoChessStopCoor(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
};

}
