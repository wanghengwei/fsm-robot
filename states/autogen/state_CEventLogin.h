
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：发送CEventLogin
class StateCEventLogin final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventLogin(QState* parent = nullptr);
private:
    void perform(std::map<std::string, std::string>& info) override;
	bool printLog() const override;
signals:
    
    void ev_CEventLoginRes_ok();
    
    void ev_CEventLoginRes_failed();
    
};

}
