
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：连接game服
class StateConnectGame final : public BasicState {
    Q_OBJECT
public:
    explicit StateConnectGame(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_ok();
    
    void ev_failed();
    
};

}
