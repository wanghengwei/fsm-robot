
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：send CEventChooseRole
class StateChooseRole : public BasicState {
    Q_OBJECT
public:
	static StateChooseRole* create(QState* parent);
    explicit StateChooseRole(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_failed();
    
    void ev_ok();
    
};

}
}
