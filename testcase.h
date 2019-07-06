#pragma once
#include <QtCore/QStateMachine>
#include <any>
#include <QtCore/QDebug>

class TestCase : public QStateMachine {
    Q_OBJECT

public:
    QStringView id() const { return m_userId; }
    void setId(QStringView id) { m_userId = id.toString(); }

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

    void setData(QStringView key, const std::any& value) {
        m_data[key.toString()] = value;
    }
private:
    QString m_userId;
    std::map<QString, std::any> m_data;
};
