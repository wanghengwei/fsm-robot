
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：
class StateCEventAutoChessNotifyClientCreateMatchRes : public BasicState {
    Q_OBJECT
public:
	static StateCEventAutoChessNotifyClientCreateMatchRes* create(QState* parent);
    explicit StateCEventAutoChessNotifyClientCreateMatchRes(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
