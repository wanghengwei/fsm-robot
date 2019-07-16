#include "send_wait_event_state.h"
#include <robot/basic_robot.h>

void SendWaitEventState::perform() {
    BasicConnection& conn = robot().connection(connectionType());
    QObject::connect(&conn, &BasicConnection::eventReceived, this, &SendWaitEventState::onEventReceived);
    doSendEvent(conn);
}

void SendWaitEventState::clean() {
    BasicConnection& conn = robot().connection(connectionType());
    conn.disconnect(this);
}

// void SendWaitEventState::onEventReceived(void* e) {
//     bool succ = true;
//     std::map<std::string, std::string> info;
//     auto sig = this->onEvent(ve, succ, info);
//     if (sig == nullptr) {
//         return;
//     }
//     if (succ) {
//         loggers::TESTCASE().info("[{}] {} OK: info={}", this->testcase().id(), this->label(), info);
//     } else {
//         loggers::TESTCASE().error("[{}] {} FAILED: info={}", this->testcase().id(), this->label(), info);
//     }
//     Q_EMIT (this->*sig)();
// }