
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：send CEventChooseRole
class StateCEventChooseRole final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventChooseRole(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_CEventChooseRoleFail();
    
    void ev_CEventJoinChannelResult_failed();
    
    void ev_CEventJoinChannelResult_ok();
    
};

}
