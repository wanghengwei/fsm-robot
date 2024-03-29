#include "basic_state.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QTimeZone>
#include <logger.h>
#include "testcase.h"

BasicState::BasicState(QState* parent) : QState{parent} {
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::VeryCoarseTimer);
}

TestCase& BasicState::testcase() {
    return *qobject_cast<TestCase*>(this->machine());
}

TestCase const& BasicState::testcase() const {
    return *qobject_cast<TestCase*>(this->machine());
}

const std::string& BasicState::id() const {
    return this->testcase().id();
}

BasicRobot& BasicState::robot() {
    return this->testcase().robot();
}

void BasicState::onEntry(QEvent* e) {
    // 在进入状态时，执行特定的行为，比如发送消息等。

    QState::onEntry(e);
    
    // 在这里执行特定行为。子类重写perform来实现

    // 执行真正的进入状态时要做的事情，比如连接服务器，发送消息等
    // std::map<std::string, std::string> info;
    this->perform();

    // if (this->printLog()) {
    //     loggers::TESTCASE().info("[{}] {}: args={}", this->testcase().id(), this->label(), info);
    // }

    if (m_timeout_ms >= 0) {
        this->m_timer.start(m_timeout_ms);
    }
}

void BasicState::onExit(QEvent* e) {
    // auto now = QDateTime::currentDateTime();
    // now = now.toOffsetFromUtc(now.offsetFromUtc());

    if (m_timeout_ms >= 0) {
        m_timer.stop();
    }

    this->clean();

    QState::onExit(e);
}

void BasicState::writeBeginLog(const std::map<std::string, std::any>& info) {
    loggers::TESTCASE().info("[{}] {}: args={}", this->testcase().id(), this->label(), info);
}

void BasicState::writeEndLogOK(const std::map<std::string, std::string>& info) {
    loggers::TESTCASE().info("[{}] {} OK: results={}", this->testcase().id(), this->label(), info);
}

void BasicState::writeEndLogFailed(const std::string& errmsg, const std::map<std::string, std::string>& info) {
    loggers::TESTCASE().error("[{}] {} FAILED: err={}, results={}", this->testcase().id(), this->label(), errmsg, info);
}

void BasicState::writeEndLogFailed(int ec, const std::map<std::string, std::string>& info) {
    loggers::TESTCASE().error("[{}] {} FAILED: err={}, results={}", this->testcase().id(), this->label(), ec, info);
}
