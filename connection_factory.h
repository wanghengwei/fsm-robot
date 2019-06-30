#pragma once
#include <QtCore/QString>
#include <QtCore/QObject>

class ConnectionFactory : QObject {
    Q_OBJECT
public:
    ConnectionFactory(bool trust) {}

    void connect(QString acc);
private:
    
};
