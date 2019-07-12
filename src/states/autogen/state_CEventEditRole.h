
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：send CEventEditRole
class StateCEventEditRole final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventEditRole(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_CEventAccountInfo();
    
    void ev_CEventEditRoleRes_nick();
    
    void ev_CEventEditRoleRes_failed();
    
};

}
