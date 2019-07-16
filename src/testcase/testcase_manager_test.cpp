#include <gtest/gtest.h>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QStateMachine>
#include <QtCore/QFinalState>

#include "testcase_manager.h"

TEST(testcase_manager_test, test_dir_join_file) {
    QDir d{"."};
    auto s = d.filePath("../aaa/bbb.xml");
    qDebug() << s;
}

TEST(testcase_manager_test, test_merge) {
    nlohmann::json j1 = R"#(
        {
            "aaa": 1
        }
    )#"_json;

    nlohmann::json j2 = R"#({
        "aaa": 2
    })#"_json;

    j1.merge_patch(j2);

    qDebug().noquote() << j1.dump().c_str();

    j1 = R"#({
        "aaa": [1, 2]
    })#"_json;

    j2 = R"#({
        "aaa": [3]
    })#"_json;

    j1.merge_patch(j2);

    qDebug().noquote() << j1.dump().c_str();

    j1 = R"#({
        "aaa": {
            "bbb": 1
        }
    })#"_json;

    j2 = R"#({
        "aaa": {
            "ccc": 2
        }
    })#"_json;

    j1.merge_patch(j2);

    qDebug().noquote() << j1.dump().c_str();

    j1 = nlohmann::json::object();

    j2 = R"#({
        "aaa": [3]
    })#"_json;

    j1.merge_patch(j2);

    qDebug().noquote() << j1.dump().c_str();
}

TEST(testcase_manager_test, test_toll) {
    std::string s = "xxx";
    char* end = nullptr;
    long long a = std::strtoll(s.data(), &end, 10);
    ASSERT_EQ(end, s.data());
    ASSERT_NE(*end, '\0');

    s = "91234567890";
    end = nullptr;
    a = std::strtoll(s.data(), &end, 10);
    ASSERT_EQ(91234567890, a);
    ASSERT_EQ('\0', *end);
}

TEST(testcase_manager_test, test_machine) {
    // return;

    int argc = 1;
    char* argv = "";
    QCoreApplication qapp{argc, &argv};

    QStateMachine m1;

    QFinalState fs(&m1);
    QObject::connect(&m1, &QStateMachine::finished, []() {
        qDebug() << "m1 finished";
    });

    QState s1{&m1};
    QObject::connect(&s1, &QState::entered, []() {
        qDebug() << "enter s1";
    });
    QTimer t1;
    s1.addTransition(&t1, &QTimer::timeout, &fs);
    t1.start(2000);

    m1.setInitialState(&s1);

    QStateMachine m2{&s1};
    s1.setInitialState(&m2);
    QObject::connect(&m2, &QStateMachine::entered, []() {
        qDebug() << "enter m2";
    });
    QObject::connect(&m2, &QStateMachine::exited, []() {
        qDebug() << "exit m2";
    });

    QState s3{&m2};
    QObject::connect(&s3, &QState::entered, []() {
        qDebug() << "enter s3";
    });

    m2.setInitialState(&s3);

    m1.start();

    qapp.exec();
}