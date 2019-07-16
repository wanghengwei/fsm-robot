
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：刷新玩家信息
class StateCEventRefreshPlayerInfo : public BasicState {
    Q_OBJECT
public:
	static StateCEventRefreshPlayerInfo* create(QState* parent);
    explicit StateCEventRefreshPlayerInfo(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_ok();
    
};

}
