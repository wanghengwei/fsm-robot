#include "base_state.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

BaseState::BaseState(QState* parent) : QState{parent} {
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::VeryCoarseTimer);
}

void BaseState::onEntry(QEvent* e) {
    if (!objectName().isEmpty()) {
        qDebug() << QDateTime::currentDateTime().toString() << this->objectName() << "enter";
    }
    QState::onEntry(e);
    
    this->perform();

    if (m_timeout_ms >= 0) {
        // qDebug() << this->objectName() << "start timer:" << m_timeout_ms << "ms";
        qDebug() << QDateTime::currentDateTime().toString() << this->objectName() << "start timer";
        this->m_timer.start(m_timeout_ms);
    }
}

void BaseState::onExit(QEvent* e) {
    if (m_timeout_ms >= 0) {
        // qDebug() << this->objectName() << "stop timer";
        qDebug() << QDateTime::currentDateTime().toString() << this->objectName() << "stop timer";
        if (m_timer.isActive()) {
            m_timer.stop();
        }
    }
    if (!objectName().isEmpty()) {
        qDebug() << QDateTime::currentDateTime().toString() << this->objectName() << "exit";
    }
}