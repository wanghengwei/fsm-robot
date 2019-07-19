
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace login {

// 状态描述：刷新玩家信息
class StateRefreshPlayerInfo : public BasicState {
    Q_OBJECT
public:
	static StateRefreshPlayerInfo* create(QState* parent);
    explicit StateRefreshPlayerInfo(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
};

}
}
