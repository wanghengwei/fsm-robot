#include <QtCore/QStateMachine>
#include <QtCore/QFinalState>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include "states/idle.h"
#include "robot.h"
#include "testcase.h"
#include <QtCore/QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include "state_factory.h"
#include <pugixml.hpp>
#include "testcase_manager.h"

int main(int argc, char** argv) {
    QCoreApplication qapp(argc, argv);

    // qDebug() << SIGNAL(ok());

    TestCaseManager testcaseManager;
    testcaseManager.create("1", "../sample-case.xml");

    testcaseManager.start();

    // QStateMachine m;
    // // QState s1{&m};
    // Idle s1{&m};
    // QObject::connect(&s1, &BasicState::ev_timeout, []() {
    //     qDebug() << QDateTime::currentDateTime().toString() << "ev_timeout triggered";
    // });
    // s1.setObjectName("idle");
    // s1.setTimeout(1000);
    // // QTimer t;
    // // QObject::connect(&s1, &QState::entered, [&]() {
    // //     qDebug("enter s1");
    // //     t.start(1000);
    // // });
    // // QObject::connect(&s1, &QState::exited, [&]() {
    // //     t.stop();
    // // });
    // s1.addTransition(&s1, &Idle::ev_timeout, &s1);

    // m.setInitialState(&s1);

    // QState s11{&s1};
    // QObject::connect(&s11, &QState::entered, [&]() {
    //     qDebug("enter s11 %i %i", s11.active(), s1.active());
    // });
    // s1.setInitialState(&s11);
    // QState s2{&m};
    // QObject::connect(&s2, &QState::entered, []() {
    //     qDebug("enter s2");
    // });

    // s11.addTransition(&s2);

    // m.start();

    qapp.exec();
    return 0;
}
