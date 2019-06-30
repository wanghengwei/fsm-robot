#include "testcase_old.h"
#include <QtCore/QState>
#include <QtCore/QDebug>
#include <QtCore/QFinalState>

#include "idle.h"
#include "connect_server.h"
#include "act_and_wait.h"
#include "states/CEventLogin.h"

// class Connect : public ActAndWait {
// public:
//     using ActAndWait::ActAndWait;

//     void on_entered() {
//         robot->connectToServer(0);
//     }
// };

void TestCase::start() {
    QState* init = new QState;
    m_machine.addState(init);
    m_machine.setInitialState(init);

    auto* finish = new QFinalState;
    m_machine.addState(finish);
    QObject::connect(&m_machine, &QStateMachine::finished, []() {
        qDebug() << "finish!";
    });

    // auto generated states
    auto connectServer = new state::ConnectServer;
    connectServer->setRobot(m_robot);
    m_machine.addState(connectServer);

    auto* sendCEventLogin = new state::CEventLogin;
    sendCEventLogin->setRobot(m_robot);
    sendCEventLogin->setTimeout(10000);
    m_machine.addState(sendCEventLogin);

    // auto generated transitions
    init->addTransition(connectServer);
    connectServer->addTransition(SIGNAL(ok()), sendCEventLogin);

    auto idle_ConnectServer_failed = new Idle{1000};
    connectServer->addTransition(SIGNAL(failed()), idle_ConnectServer_failed);
    idle_ConnectServer_failed->addTransition(connectServer);

    sendCEventLogin->addTransition(SIGNAL(recv_CEventRes1()), finish);

    // Idle* idle = new Idle{2000};
    // m_machine.addState(&idle->state);

    // init->addTransition(&idle->state);

    // idle->addTransition(&idle->state);    

    // custom
    // QState* sleeping_1 = new QState;
    // m_machine.addState(sleeping_1);
    // QObject::connect(sleeping_1, &QState::entered, [this]() {
    //     qDebug() << "enter sleeping_1";
    //     this->m_timer.setSingleShot(true);
    //     this->m_timer.start(1000);
    // });

    // init->addTransition(sleeping_1);
    // sleeping_1->addTransition(&m_timer, &QTimer::timeout, sleeping_1);

    //
    m_machine.start();
}