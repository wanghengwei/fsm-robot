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

    TestCaseManager testcaseManager;
    testcaseManager.setSpeedRate(100);
    testcaseManager.createMany("100000", 1, "../sample-case.xml");
    // testcaseManager.create("1", "../sample-case.xml");
    // testcaseManager.create("2", "../sample-case.xml");

    testcaseManager.start();

    qapp.exec();
    return 0;
}
