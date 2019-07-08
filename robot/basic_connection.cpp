#include "basic_connection.h"
#include <logger.h>
#include <QtCore/QTimer>

void FakeConnection::connect(const std::string& id, const std::string& ip) {
    loggers::NET().info("[{}] connect to {}", id, ip);
    QTimer::singleShot(1000, Qt::VeryCoarseTimer, this, &FakeConnection::connectOK);
}

void FakeConnection::sendEvent(void* e) {
    if (e == nullptr) {
        QTimer::singleShot(500, Qt::VeryCoarseTimer, this, &FakeConnection::kicked);
        return;
    }

    QTimer::singleShot(1000, Qt::VeryCoarseTimer, this, [=]() {
        emit this->eventReceived(e);
    });
}
