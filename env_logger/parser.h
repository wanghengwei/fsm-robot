#pragma once
#include <QtCore/QString>
#include "spdlog/common.h"
#include <QtCore/QMap>

namespace env_logger {
    bool parseLoggerSetString(const QString& s, QMap<QString, spdlog::level::level_enum>& r);
}