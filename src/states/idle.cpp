#include "idle.h"
#include <QtCore/QDebug>

Idle::Idle(QState* parent) : BasicState{parent} {
    QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
}

void Idle::perform(std::map<std::string, std::string>& info) {
    
}
