
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：wait CEventConnectRes
class StateCEventConnectRes : public BasicState {
    Q_OBJECT
public:
	static StateCEventConnectRes* create(QState* parent);
    explicit StateCEventConnectRes(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_CEventConnectRes_ok();
    
    void ev_CEventConnectRes_failed();
    
};

}
