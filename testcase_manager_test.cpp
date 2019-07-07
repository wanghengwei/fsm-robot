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