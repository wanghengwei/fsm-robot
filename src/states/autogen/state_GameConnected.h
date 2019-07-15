
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：已连接Game
class StateGameConnected final : public BasicState {
    Q_OBJECT
public:
    explicit StateGameConnected(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_kicked();
    
};

}
