
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：send CEventClientLoadAccountInfo
class StateCEventClientLoadAccountInfo : public BasicState {
    Q_OBJECT
public:
	static StateCEventClientLoadAccountInfo* create(QState* parent);
    explicit StateCEventClientLoadAccountInfo(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_CEventAccountInfo_noRole();
    
    void ev_CEventAccountInfo_ok();
    
};

}
