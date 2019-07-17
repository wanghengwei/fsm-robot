#pragma once
#include <QtCore/QString>
// #include <QtCore/QStringView>
#include <map>
#include <any>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

template<>
struct fmt::formatter<QString> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const QString &s, FormatContext &ctx) {
        return format_to(ctx.out(), "{}", qPrintable(s));
    }
};

// // template<typename OStream>
// std::ostream &operator<<(std::ostream& os, const QString& s) {
//     return os << qPrintable(s);
// }

template<>
struct fmt::formatter<std::any> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const std::any& s, FormatContext &ctx) {
        auto& t =s.type();
        if (t == typeid(int)) {
            return format_to(ctx.out(), "{}", std::any_cast<int>(s));
        } else if (t == typeid(long long)) {
            return format_to(ctx.out(), "{}", std::any_cast<long long>(s));
        } else if (t == typeid(std::string)) {
            return format_to(ctx.out(), "{}", std::any_cast<const std::string&>(s));
        } else {
            return format_to(ctx.out(), "<ANY>");
        }
    }
};

template<typename K, typename V>
struct fmt::formatter<std::map<K, V>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const std::map<K, V>& s, FormatContext &ctx) {
        format_to(ctx.out(), "{{");
        int i = 0;
        const int max = 5;
        int n = s.size();
        for (auto& p : s) {
            format_to(ctx.out(), "{}:{}", p.first, p.second);
            if (i < n - 1) {
                format_to(ctx.out(), ",");
            }
            ++i;
            if (i >= max && i < n - 1) {
                format_to(ctx.out(), "...");
                break;
            }
        }
        return format_to(ctx.out(), "}}");
    }
};


// template<typename OStream>
// OStream &operator<<(OStream &os, const std::any& s) {
//     auto& t =s.type();
//     if (t == typeid(int)) {
//         return os << std::any_cast<int>(s);
//     } else if (t == typeid(long long)) {
//         return os << std::any_cast<long long>(s);
//     } else if (t == typeid(std::string)) {
//         return os << std::any_cast<const std::string&>(s);
//     } else {
//         return os << "<ANY>";
//     }
// }

// template<typename OStream, typename K, typename V>
// OStream &operator<<(OStream &os, std::map<K, V> const& s) {
//     os << "{";
//     int i = 0;
//     const int max = 5;
//     int n = s.size();
//     for (auto& p : s) {
//         os << p.first << ":" << p.second;
//         if (i < n - 1) {
//             os << ",";
//         }
//         ++i;
//         if (i >= max && i < n - 1) {
//             os << "...";
//             break;
//         }
//     }
//     os << "}";
//     return os;
// }

namespace loggers {
    spdlog::logger& MAIN();
    spdlog::logger& TESTCASE_MANAGER();
    spdlog::logger& TESTCASE_PARSER();
    spdlog::logger& TESTCASE();
    spdlog::logger& NET();
}
