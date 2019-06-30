#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QStateMachine>
#include "cevents.h"

class Robot : public QObject {
    Q_OBJECT
public:
    explicit Robot(QObject* parent = nullptr);

    void connectServer(int serviceID);

    void sendEvent(CEvent* ev);
signals:
    // void connect();
    void connect_failed();
    void connect_ok();
    void be_kicked();
    void disconnect();
    void disconnect_ok();

    // 表示一个事件，让机器人发送一个cevent
    void ev_send_CEventLogin();

    void ev_timeout();

    void eventReceived(CEvent& ev);

public:
    void send_CEventLogin();

public:
    // void timerEvent(QTimerEvent* ev) override;
    void on_enter_connecting();

    void send_event(CEvent* ev);

public:
    void start();
protected:
    void timerEvent(QTimerEvent* e) override;
private:
    QStateMachine* machine;
};
