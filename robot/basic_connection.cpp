#include "basic_connection.h"
#include <logger.h>
#include <QtCore/QTimer>

void FakeConnection::connect(const std::string& id, const std::string& ip) {
    loggers::NET().info("[{}] connect to {}", id, ip);
    QTimer::singleShot(1000, this, &FakeConnection::connectOK);
}

void FakeConnection::sendEvent(void* e) {
    QTimer::singleShot(1000, this, [=]() {
        emit this->eventReceived(e);
    });
}
