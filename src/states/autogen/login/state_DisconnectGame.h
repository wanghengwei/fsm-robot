
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：断开game连接
class StateDisconnectGame : public BasicState {
    Q_OBJECT
public:
	static StateDisconnectGame* create(QState* parent);
    explicit StateDisconnectGame(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
};

}
}
