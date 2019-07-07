#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "parser.h"
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDebug>

#include <iostream>

QMap<QString, spdlog::level::level_enum> getEnvSetting() {
    auto v = QProcessEnvironment::systemEnvironment().value("H3D_LOG");  
    QMap<QString, spdlog::level::level_enum> m;
    env_logger::parseLoggerSetString(v, m);
    return m;
}

std::shared_ptr<spdlog::logger> initLogger(const char* moduleName) {
    static auto m = getEnvSetting();
    auto p = spdlog::stdout_color_st(moduleName);
    auto lv = m.value(moduleName, spdlog::level::info);
    p->set_level(lv);
    p->set_pattern("[%Y-%m-%dT%H:%M:%S.%e%z] %^[%L]%$ [%n] %v");

    // qDebug().noquote() << "[log] set" << moduleName << "to" << lv << spdlog::level::to_string_view(lv).data();

    return p;
}

namespace loggers {
    spdlog::logger& MAIN() {
        static auto lg = initLogger("main");
        return *lg;
    } 

    spdlog::logger& TESTCASE_MANAGER() {
        static auto lg = initLogger("tc.manager");
        return *lg;
    }

    spdlog::logger& TESTCASE_PARSER() {
        static auto lg = initLogger("tc.parser");
        return *lg;
    }

    spdlog::logger& TESTCASE() {
        static auto lg = initLogger("tc");
        return *lg;
    }

    spdlog::logger& NET() {
        static auto lg = initLogger("net");
        return *lg;
    }
}
