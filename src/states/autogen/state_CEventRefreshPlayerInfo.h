
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：刷新玩家信息
class StateCEventRefreshPlayerInfo final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventRefreshPlayerInfo(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_ok();
    
};

}
