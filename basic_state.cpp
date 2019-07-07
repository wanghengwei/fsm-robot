#include "basic_state.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QTimeZone>
#include <logger.h>

BasicState::BasicState(QState* parent) : QState{parent} {
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::VeryCoarseTimer);
}

TestCase& BasicState::testcase() {
    return *qobject_cast<TestCase*>(this->machine());
}

void BasicState::onEntry(QEvent* e) {
    // 在进入状态时，执行特定的行为，比如发送消息等。

    QState::onEntry(e);
    
    // 在这里执行特定行为。子类重写perform来实现

    // 执行真正的进入状态时要做的事情，比如连接服务器，发送消息等
    std::map<std::string, std::string> info;
    this->perform(info);

    if (this->printLog()) {
        loggers::TESTCASE().info("[{}] {}: args={}", this->testcase().id(), this->label(), info);
    }

    if (m_timeout_ms >= 0) {
        this->m_timer.start(m_timeout_ms);
    }
}

void BasicState::onExit(QEvent* e) {
    auto now = QDateTime::currentDateTime();
    now = now.toOffsetFromUtc(now.offsetFromUtc());

    if (m_timeout_ms >= 0) {
        m_timer.stop();
    }
}