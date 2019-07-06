#pragma once
#include <QtCore/QString>
#include <QtCore/QStringView>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

template<typename OStream>
OStream &operator<<(OStream &os, const QString& s) {
    return os << qPrintable(s);
}

template<typename OStream>
OStream &operator<<(OStream &os, QStringView s) {
    return os << s.toLocal8Bit().constData();
}

namespace loggers {
    spdlog::logger& MAIN();
    spdlog::logger& TESTCASE_MANAGER();
    spdlog::logger& TESTCASE_PARSER();
    spdlog::logger& TESTCASE();
}
