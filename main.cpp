#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include "testcase_manager.h"
#include <logger.h>

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

    TestCaseManager testcaseManager;
    testcaseManager.setTestCaseDir(parser.value("testcase-dir"));
    testcaseManager.setSpeedRate(100);
    testcaseManager.loadUserData(parser.value("data-dir"));
    testcaseManager.createMany(firstId, count, testcasePath);

    testcaseManager.start();

    qapp.exec();
    return 0;
}
