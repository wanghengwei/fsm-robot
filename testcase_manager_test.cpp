#include <gtest/gtest.h>
#include <QtCore/QDir>
#include <QtCore/QDebug>
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