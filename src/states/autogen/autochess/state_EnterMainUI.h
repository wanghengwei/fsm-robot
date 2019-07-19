
#pragma once
#include <testcase/basic_state.h>

namespace state {
namespace autochess {

// 状态描述：
class StateEnterMainUI : public BasicState {
    Q_OBJECT
public:
	static StateEnterMainUI* create(QState* parent);
    explicit StateEnterMainUI(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    
    void ev_reconnect_coordinated();
    
    void ev_reconnect_ingame();
    
    void ev_ok();
    
    void ev_failed();
    
};

}
}
