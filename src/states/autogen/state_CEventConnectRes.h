
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：wait CEventConnectRes
class StateCEventConnectRes final : public BasicState {
    Q_OBJECT
public:
    explicit StateCEventConnectRes(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
Q_SIGNALS:
    
    void ev_CEventConnectRes_ok();
    
    void ev_CEventConnectRes_failed();
    
};

}
