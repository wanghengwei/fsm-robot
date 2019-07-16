
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：连接game服
class StateConnectGame : public BasicState {
    Q_OBJECT
public:
	static StateConnectGame* create(QState* parent);
    explicit StateConnectGame(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
