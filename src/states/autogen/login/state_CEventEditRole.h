
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：send CEventEditRole
class StateCEventEditRole : public BasicState {
    Q_OBJECT
public:
	static StateCEventEditRole* create(QState* parent);
    explicit StateCEventEditRole(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_CEventAccountInfo();
    
    void ev_CEventEditRoleRes_nick();
    
    void ev_CEventEditRoleRes_failed();
    
};

}
