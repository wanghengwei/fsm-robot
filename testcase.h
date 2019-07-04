#pragma once
#include <QtCore/QStateMachine>
#include <any>
#include <QtCore/QDebug>

class TestCase : public QStateMachine {
    Q_OBJECT

public:
    template<typename T>
    bool __attribute_warn_unused_result__ getData(const QString& key, T& value) {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            return false;
        }

        try {
            value = std::any_cast<T>(it->second);
            return true;
        } catch (std::bad_any_cast const& ex) {
            qDebug() << "cannot get data with key" << key << ":" << ex.what();
            return false;
        }
    }
private:
    std::map<QString, std::any> m_data;
};
