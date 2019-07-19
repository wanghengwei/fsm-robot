
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：
class StateCEventAutoChessEnterMainUI : public BasicState {
    Q_OBJECT
public:
	static StateCEventAutoChessEnterMainUI* create(QState* parent);
    explicit StateCEventAutoChessEnterMainUI(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_reconnect_coordinated();
    
    void ev_reconnect_ingame();
    
    void ev_ok();
    
    void ev_failed();
    
};

}
