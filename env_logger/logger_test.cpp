#include <gtest/gtest.h>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDebug>
#include "parser.h"

TEST(logger_test, test_env) {
    auto v = QProcessEnvironment::systemEnvironment().value("H3D_LOG");
    qDebug() << v;    
    QMap<QString, spdlog::level::level_enum> m;
    env_logger::parseLoggerSetString(v, m); 
    qDebug() << m;
}
