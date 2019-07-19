
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：已连接Game
class StateGameConnected : public BasicState {
    Q_OBJECT
public:
	static StateGameConnected* create(QState* parent);
    explicit StateGameConnected(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_kicked();
    
};

}
}
