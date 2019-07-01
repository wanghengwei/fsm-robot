
#pragma once
#include <base_state.h>

namespace state {

// 状态描述：断开game连接
class StateDisconnectGame final : public BaseState {
    Q_OBJECT
public:
    explicit StateDisconnectGame(QState* parent = nullptr);
private:
    void perform() override;

signals:
    
};

}
