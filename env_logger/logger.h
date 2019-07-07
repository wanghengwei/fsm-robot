#pragma once
#include <QtCore/QString>
#include <QtCore/QStringView>
#include <map>
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

template<typename OStream, typename K, typename V>
OStream &operator<<(OStream &os, std::map<K, V> const& s) {
    os << "{";
    int i = 0;
    const int max = 5;
    int n = s.size();
    for (auto& p : s) {
        os << p.first << ":" << p.second;
        if (i < n - 1) {
            os << ",";
        }
        ++i;
        if (i >= max && i < n - 1) {
            os << "...";
            break;
        }
    }
    os << "}";
    return os;
}

namespace loggers {
    spdlog::logger& MAIN();
    spdlog::logger& TESTCASE_MANAGER();
    spdlog::logger& TESTCASE_PARSER();
    spdlog::logger& TESTCASE();
}
