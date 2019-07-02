
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：在房间内
class StateInRoom final : public BasicState {
    Q_OBJECT
public:
    explicit StateInRoom(QState* parent = nullptr);
private:
    void perform() override;

signals:
    
    void ev_leaveRoom();
    
};

}
