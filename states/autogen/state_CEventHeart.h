
#pragma once
#include <base_state.h>

namespace state {

// 状态描述：发心跳
class StateCEventHeart final : public BaseState {
    Q_OBJECT
public:
    explicit StateCEventHeart(QState* parent = nullptr);
private:
    void perform() override;

signals:
    
};

}
