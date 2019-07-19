
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：send CEventClientLoadAccountInfo
class StateLoadAccountInfo : public BasicState {
    Q_OBJECT
public:
	static StateLoadAccountInfo* create(QState* parent);
    explicit StateLoadAccountInfo(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_no_role();
    
    void ev_ok();
    
};

}
}
