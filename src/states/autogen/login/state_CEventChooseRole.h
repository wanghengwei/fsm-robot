
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：send CEventChooseRole
class StateCEventChooseRole : public BasicState {
    Q_OBJECT
public:
	static StateCEventChooseRole* create(QState* parent);
    explicit StateCEventChooseRole(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_CEventChooseRoleFail();
    
    void ev_CEventJoinChannelResult_failed();
    
    void ev_CEventJoinChannelResult_ok();
    
};

}
