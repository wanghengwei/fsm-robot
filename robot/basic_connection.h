#pragma once
#include <string>
#include <QtCore/QObject>

class BasicConnection : public QObject {
    Q_OBJECT
public:
    virtual ~BasicConnection() {}

    virtual void connect(const std::string& id, const std::string& ip) = 0;

    virtual void sendEvent(void* e) = 0;

signals:
    void eventReceived(void *);

    void connectOK();
    void connectFailed();

    void kicked();
};

class FakeConnection : public BasicConnection {
public:
    void connect(const std::string& id, const std::string& ip) override;
    void sendEvent(void* e) override;
};