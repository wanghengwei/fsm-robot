#include "basic_connection.h"
#include <logger.h>
#include <QtCore/QTimer>

void FakeConnection::connect(const std::string& ip, int /*port*/, const std::string& id) {
    loggers::NET().info("[{}] connect to {}", id, ip);
    QTimer::singleShot(1000, Qt::VeryCoarseTimer, this, &FakeConnection::connectOK);
}

void FakeConnection::sendEvent(void* e) {
    if (e == nullptr) {
        QTimer::singleShot(500, Qt::VeryCoarseTimer, this, &FakeConnection::kicked);
        return;
    }

    QTimer::singleShot(1000, Qt::VeryCoarseTimer, this, [=]() {
        Q_EMIT this->eventReceived(e);
    });
}
