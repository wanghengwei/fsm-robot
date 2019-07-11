#include "parser.h"
#include <QtCore/QDebug>

namespace env_logger {

    struct parser {
        QString content;
        int index = 0;
        QMap<QString, spdlog::level::level_enum> result;
        bool ok = true;

        bool more() const {
            return ok && index < content.length();
        }

        QChar cur() const {
            return content[index];
        }

        bool isValidNameChar() {
            auto c = cur();
            return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '.');
        }

        spdlog::level::level_enum parseLevel() {
            if (!more()) {
                ok = false;
                return spdlog::level::off;
            }

            QStringRef sr{&content, index, content.length() - index};

            static auto levels = {
                QString{"trace"},
                QString{"debug"},
                QString{"info"},
                QString{"warning"},
                QString{"error"},
                QString{"critical"},
                QString{"off"},
            };

            for (auto& lv : levels) {
                if (sr.startsWith(lv)) {
                    index += lv.length();
                    return spdlog::level::from_str(qPrintable(lv));
                }
            }

            ok = false;
            return spdlog::level::off;
        }

        QString parseName() {
            if (!more()) {
                ok = false;
                return "";
            }

            if (!isValidNameChar()) {
                ok = false;
                return "";
            }

            int start = index;
            ++index;

            while(index < content.length() && isValidNameChar()) {
                ++index;
            }

            auto name = content.mid(start, index - start);
            return name;
        }

        void parseSegment() {
            if (!more()) {
                ok = false;
                return;
            }

            QString name = parseName();
            if (!more()) {
                ok = false;
                return;
            }
            if (cur() != '=') {
                ok = false;
                return;
            }
            ++index;
            auto lv = parseLevel();
            if (!ok) {
                return;
            }

            result[name] = lv;
        }

        void parseAll() {
            if (content.isEmpty()) {
                ok = false;
                return;
            }

            auto c = cur();
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
                parseSegment();
                if (!more()) {
                    return;
                }
                if (cur() != ',') {
                    ok = false;
                    return;
                }
                ++index;
                parseAll();
            }
            // parseSegment(s, r);
        }
    };

    // all = segment || segment,all
    // segment = name=level
    // name = [a-zA-Z0-9.]+
    // level = trace|debug|info|warning|error|critical|off
    bool parseLoggerSetString(const QString& s, QMap<QString, spdlog::level::level_enum>& r) {
        parser p;
        p.content = s;
        p.parseAll();
        r = p.result;
        return p.ok;
    }
}