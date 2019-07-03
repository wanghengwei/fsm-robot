#include "basic_state.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QTimeZone>

BasicState::BasicState(QState* parent) : QState{parent} {
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::VeryCoarseTimer);
}

void BasicState::onEntry(QEvent* e) {
    // 在进入状态时，执行特定的行为，比如发送消息等。

    // if (!objectName().isEmpty()) {
    //     qDebug() << QDateTime::currentDateTime().toString() << this->objectName() << "enter";
    // }
    QState::onEntry(e);
    
    // 在这里执行特定行为。子类重写perform来实现
    auto now = QDateTime::currentDateTime();
    now = now.toOffsetFromUtc(now.offsetFromUtc());
    if (this->printLog()) {
        qDebug().noquote() << now.toString(Qt::ISODateWithMs) << this->label() << "begin";
    }
    this->perform();
    if (printLog()) {
        qDebug().noquote() << now.toString(Qt::ISODateWithMs) << this->label() << "end";
    }

    if (m_timeout_ms >= 0) {
        // qDebug().noquote() << now.toString(Qt::ISODateWithMs) << this->objectName() << "start timer" << m_timeout_ms;
        this->m_timer.start(m_timeout_ms);
    }
}

void BasicState::onExit(QEvent* e) {
    auto now = QDateTime::currentDateTime();
    now = now.toOffsetFromUtc(now.offsetFromUtc());

    if (m_timeout_ms >= 0) {
        // qDebug().noquote() << now.toString(Qt::ISODateWithMs) << this->objectName() << "stop timer";
        // if (m_timer.isActive()) {
        m_timer.stop();
        // }
    }

    // if (!objectName().isEmpty()) {
    //     qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << this->objectName() << "exit";
    // }
}