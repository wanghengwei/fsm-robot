#pragma once
#include <base_state.h>

namespace state {

// 状态描述：{{ description }}
class {{ classname }} final : public BaseState {
    Q_OBJECT
public:
    explicit {{ classname }}(QState* parent = nullptr);
private:
    void perform() override;

signals:
    {% for signal in signals %}
    void ev_{{ signal }}();
    {% endfor %}
};

}
