#pragma once
#include "connection_factory.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>

void ConnectionFactory::connect(QString acc) {
    qDebug() << "connect: account=" << acc;
    QTimer::singleShot(1000, this, [=]() {});
}
