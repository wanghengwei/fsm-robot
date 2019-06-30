#include "robot.h"
#include <QtCore/QDebug>
#include <QtCore/QFinalState>

Robot::Robot(QObject* parent) : QObject{parent} {
    // // 机器人初始化时需要初始化一堆状态，但初始化什么状态应该由用例决定
    // machine = new QStateMachine{this};

    // QFinalState* end = new QFinalState;
    // machine->addState(end);

    // QState* init = new QState;
    // machine->addState(init);
    // machine->setInitialState(init);

    // QState* connecting = new QState;
    // machine->addState(connecting);

    // QState* connectFailed = new QState;
    // machine->addState(connectFailed);

    // QState* connected = new QState;
    // machine->addState(connected);

    // QState* beKicked = new QState;
    // machine->addState(beKicked);

    // QState* disconnecting = new QState;
    // machine->addState(disconnecting);

    // QState* disconnected = new QState;
    // machine->addState(disconnected);

    // // QState* 

    // QState* sentCEventLogin = new QState{connected};
    // // machine->addState(sentCEventLogin);
    // connected->setInitialState(sentCEventLogin);

    // // 按道理，转换应当由用例来决定。不过有的转换比较固定
    // QObject::connect(machine, &QStateMachine::finished, []() {
    //     qDebug() << "end!";
    // });

    // init->addTransition(this, &Robot::connect, connecting);

    // // 当进入connecting状态时一定要去调用网络connect函数，这是固定的，不应该让每个用例都去自己指定
    // QObject::connect(connecting, &QState::entered, this, &Robot::on_enter_connecting);
    // connecting->addTransition(this, &Robot::connect_failed, connectFailed);
    // connecting->addTransition(this, &Robot::connect_ok, connected);

    // connectFailed->addTransition(end);

    // connected->addTransition(this, &Robot::be_kicked, beKicked);
    // connected->addTransition(this, &Robot::disconnect, disconnecting);
    // // connected->addTransition(this, &Robot::ev_send_CEventLogin, sentCEventLogin);
    // // QObject::connect(connected, &QState::entered, [this]() {
    // //     // 这一段代码是由用户决定的
    // //     QTimer::singleShot(1000, [this]() {
    // //         emit ev_send_CEventLogin();
    // //         // this->send_event(new CEventLogin);
    // //     });
    // // });

    // // custom
    // sentCEventLogin->addTransition(this, &Robot::ev_timeout, sentCEventLogin);
    // // 状态 sentXXX 进入时，调用 send_XXX 来实际发送消息
    // QObject::connect(sentCEventLogin, &QState::entered, this, &Robot::send_CEventLogin);

    // // customized by user:
    // // beKicked->addTransition(end);
    // beKicked->addTransition(connecting);

    // disconnecting->addTransition(this, &Robot::disconnect_ok, disconnected);

    // disconnected->addTransition(end);
}

void Robot::on_enter_connecting() {
    qDebug() << "robot connect to server";
    QTimer* t = new QTimer{this};
    // 假设成功
    QObject::connect(t, &QTimer::timeout, this, [this, t]() {
        qDebug() << "connect ok!";
        t->stop();
        t->deleteLater();
        emit connect_ok();
    });
    t->start(1000);
    // emit do_connect();
}

// void Robot::timerEvent(QTimerEvent* ev) {
//     emit do_connect();
// }

void Robot::start() {
    qDebug() << "start state machine";
    machine->start();
}

void Robot::send_CEventLogin() {
    qDebug() << "send CEventLogin";
    CEventLogin event;
    event.account = "";
    this->send_event(&event);

    // 如果需要回复消息，那么就开始一个计时
    // QTimer::singleShot(3000, this, &Robot::ev_timeout);
    this->startTimer(3000);
}

void Robot::send_event(CEvent* ev) {
    // qDebug() << "sending event";
}

void Robot::timerEvent(QTimerEvent* e) {
    killTimer(e->timerId());
    emit ev_timeout();
}

void Robot::sendEvent(CEvent* ev) {
    CEventRes1 rev;
    emit eventReceived(rev);    
}

void Robot::connectServer(int serviceID) {
    emit connect_ok();
}