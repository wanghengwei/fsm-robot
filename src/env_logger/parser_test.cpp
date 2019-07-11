#include <gtest/gtest.h>
#include <QtCore/QString>
#include "parser.h"

TEST(parser_test, test_normal) {
    QString a = "a=info,b=debug,c.d.e=error";
    QMap<QString, spdlog::level::level_enum> expected {
        {"a", spdlog::level::info},
        {"b", spdlog::level::debug},
        {"c.d.e", spdlog::level::err},
    };

    QMap<QString, spdlog::level::level_enum> actual;
    bool ok = env_logger::parseLoggerSetString(a, actual);

    ASSERT_TRUE(ok);

    ASSERT_EQ(expected, actual);
}

TEST(parser_test, test_error) {
    QString a = "a=infoaaa";
    QMap<QString, spdlog::level::level_enum> actual;
    bool ok = env_logger::parseLoggerSetString(a, actual);
    ASSERT_FALSE(ok);

    ok = env_logger::parseLoggerSetString("asdfsfinfo", actual);
    ASSERT_FALSE(ok);

    ok = env_logger::parseLoggerSetString("", actual);
    ASSERT_FALSE(ok);

    ok = env_logger::parseLoggerSetString("aaaa=xxx", actual);
    ASSERT_FALSE(ok);

    ok = env_logger::parseLoggerSetString("asdf = info", actual);
    ASSERT_FALSE(ok);
}