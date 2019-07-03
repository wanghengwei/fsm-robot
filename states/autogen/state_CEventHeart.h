
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：发心跳
class StateCEventHeart final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventHeart(QState* parent = nullptr);
private:
    void perform() override;
	bool printLog() const override;
signals:
    
};

}
