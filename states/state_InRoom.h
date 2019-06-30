#pragma once
#include <base_state.h>

namespace state {

// 状态描述：在房间内
class StateInRoom final : public BaseState {
    Q_OBJECT
public:
    explicit StateInRoom(QState* parent = nullptr);
private:
    void perform() override;

signals:
    
    void ev_leaveRoom();
    
};

}