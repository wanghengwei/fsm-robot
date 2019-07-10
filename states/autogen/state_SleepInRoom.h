
#pragma once
#include <basic_state.h>

namespace state {

// 状态描述：在房间内发呆
class StateSleepInRoom final : public BasicState {
    Q_OBJECT
public:
    explicit StateSleepInRoom(QState* parent = nullptr);
private:
	void perform(std::map<std::string, std::string>& info) override;
	void clean() override;
	bool printLog() const override;
signals:
    
};

}
