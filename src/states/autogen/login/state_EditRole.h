
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：send CEventEditRole
class StateEditRole : public BasicState {
    Q_OBJECT
public:
	static StateEditRole* create(QState* parent);
    explicit StateEditRole(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_nick_error();
    
    void ev_failed();
    
};

}
}
