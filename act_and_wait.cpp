#include "act_and_wait.h"

ActAndWait::ActAndWait(std::function<void()> act, int to) {
    timer.setTimerType(Qt::VeryCoarseTimer);
    timer.setSingleShot(true);
    QObject::connect(&state, &QState::entered, [=]() {
        act();
        this->timer.start(to);
    });
}

void ActAndWait::setTimeoutTransition(QAbstractState* s) {
    this->state.addTransition(&timer, &QTimer::timeout, s);
}