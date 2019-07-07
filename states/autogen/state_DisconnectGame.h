
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：断开game连接
class StateDisconnectGame final : public BasicState {
    Q_OBJECT
public:
    explicit StateDisconnectGame(QState* parent = nullptr);
private:
    void perform(std::map<std::string, std::string>& info) override;
	bool printLog() const override;
signals:
    
};

}