#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <logger.h>
#include "testcase_manager.h"
#include <robot/connection_factory.h>
#include <robot/x51_connection_factory.h>
#include <robot/x51_netclient_manager.h>
#include <share/netengine/net.h>

int main(int argc, char** argv) {

    QCoreApplication qapp(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({
        "f", "first id", "FIRST_ID", "1000000"
    });
    parser.addOption({
        "n", "count of robot", "COUNT", "1"
    });
    parser.addOption({
        "t", "path of testcase", "TESTCASE_PATH", ""
    });
    parser.addOption({
        "testcase-dir", "base dir of testcases", "DIR", "."
    });
    parser.addOption({
        "data-dir", "base dir of data", "DIR", "./user_data"
    });

    parser.process(qapp);

    // loggers::MAIN().info("first={}", parser.value("f"));
    bool ok = true;
    QString firstId = parser.value("f");
    firstId.toULongLong(&ok);
    if (!ok) {
        loggers::MAIN().error("firstId must be long, but {}", firstId);
        return 1;
    }
    int count = parser.value("n").toUInt(&ok);
    if (!ok) {
        loggers::MAIN().error("count must be uint, but {}", parser.value("n"));
        return 1;
    }
    QString testcasePath = parser.value("t");
    if (testcasePath.isEmpty()) {
        loggers::MAIN().error("-t must not be empty");
        return 1;
    }

    // =============================================================

    GetLogInterface()->SetSystemPriority(200);
    IEventSelector* es = GetBiboRegistry()->CreateEventSelector();

    std::shared_ptr<x51::NetClientManager> netClientManager{new x51::NetClientManager{es}};

    // std::shared_ptr<BasicConnectionFactory> connectionFactory{new FakeConnectionFactory};
    std::shared_ptr<BasicConnectionFactory> connectionFactory{new x51::ConnectionFactory{netClientManager}};
    
    TestCaseManager testcaseManager;
    testcaseManager.setConnectionFactory(connectionFactory);
    testcaseManager.setTestCaseDir(parser.value("testcase-dir").toStdString());
    testcaseManager.setSpeedRate(10);
    testcaseManager.loadUserData(parser.value("data-dir"));
    testcaseManager.createMany(firstId.toStdString(), count, testcasePath);

    QTimer estimer;
    QObject::connect(&estimer, &QTimer::timeout, [es]() {
        es->ProcessEvents(0);
    });
    estimer.start(100);

    testcaseManager.start();

    qapp.exec();
    return 0;
}
