#pragma once
#include <string>
#include <QtCore/QObject>

class BasicConnection : public QObject {
    Q_OBJECT
public:
    virtual ~BasicConnection() {}

    virtual void connect(const std::string& ip, int port, const std::string& id) = 0;

    virtual void close() = 0;

    virtual void sendEvent(void* e) = 0;

Q_SIGNALS:
    void eventReceived(void *);

    void connectOK();
    void connectFailed(const std::string& msg);

    void kicked();
};

class FakeConnection : public BasicConnection {
public:
    void connect(const std::string& ip, int port, const std::string& id) override;
    void close() override {}
    void sendEvent(void* e) override;
};