
#pragma once
#include <base_state.h>

namespace state {

// 状态描述：发送CEventLogin
class StateCEventLogin final : public BaseState {
    Q_OBJECT
public:
    explicit StateCEventLogin(QState* parent = nullptr);
private:
    void perform() override;

signals:
    
    void ev_CEventLoginRes_ok();
    
    void ev_CEventLoginRes_failed();
    
};

}
