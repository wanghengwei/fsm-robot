
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：wait CEventConnectRes
class StateWaitConnectRes : public BasicState {
    Q_OBJECT
public:
	static StateWaitConnectRes* create(QState* parent);
    explicit StateWaitConnectRes(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
}
