
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：已登录
class StateLogined : public BasicState {
    Q_OBJECT
public:
	static StateLogined* create(QState* parent);
    explicit StateLogined(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
};

}
}
