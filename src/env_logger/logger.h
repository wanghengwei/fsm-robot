#pragma once
#include <QtCore/QString>
// #include <QtCore/QStringView>
#include <map>
#include <any>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

template<typename OStream>
OStream &operator<<(OStream &os, const QString& s) {
    return os << qPrintable(s);
}

template<typename OStream>
OStream &operator<<(OStream &os, const std::any& s) {
    auto& t =s.type();
    if (t == typeid(int)) {
        return os << std::any_cast<int>(s);
    } else if (t == typeid(long long)) {
        return os << std::any_cast<long long>(s);
    } else if (t == typeid(std::string)) {
        return os << std::any_cast<const std::string&>(s);
    } else {
        return os << "<ANY>";
    }
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
    spdlog::logger& NET();
}
