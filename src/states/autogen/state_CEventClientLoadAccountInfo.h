
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：send CEventClientLoadAccountInfo
class StateCEventClientLoadAccountInfo final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventClientLoadAccountInfo(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_CEventAccountInfo_noRole();
    
    void ev_CEventAccountInfo_ok();
    
};

}
