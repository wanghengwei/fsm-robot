#include <gtest/gtest.h>
#include <QtCore/QDir>
#include <QtCore/QDebug>

TEST(testcase_manager_test, test_dir_join_file) {
    QDir d{"."};
    auto s = d.filePath("../aaa/bbb.xml");
    qDebug() << s;
}