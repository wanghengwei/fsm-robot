#include "idle.h"
#include <QtCore/QDebug>

Idle::Idle(QState* parent) : BaseState{parent} {
    QObject::connect(this->timer(), &QTimer::timeout, this, &BaseState::ev_timeout);
}

void Idle::perform() {
    
}
