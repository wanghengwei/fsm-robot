#pragma once
#include "../base_state.h"
#include <QtCore/QTimer>

class Idle final : public BaseState {
    Q_OBJECT
public:
    explicit Idle(QState* parent = nullptr);

    // void setTimeout()

    // void addTransition(QAbstractState* state);

    // QState state;
private:
    void perform() override;
//     void onEntry(QEvent* e) override;
// private:
//     QTimer timer;
//     int m_timeout_ms = 0;
};