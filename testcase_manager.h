#pragma once
#include <map>
#include <memory>
#include <queue>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <pugixml.hpp>

class TestCase;

class TestCaseManager : public QObject {
    Q_OBJECT
public:
    TestCaseManager();

    void setSpeedRate(int countPerSecond) { m_speed = countPerSecond; }
    void setBaseDir(QStringView dir) { m_testcaseBaseDir = dir.toString(); }

    void start();
    
    bool create(const QString& id, const QString& caseName);
    bool create(const QString& id, const pugi::xml_document& doc);
    void createMany(const QString& first, int count, const QString& caseName);

private:
    void startSome();
private:
    std::map<QString, std::shared_ptr<TestCase>> m_testcases;
    std::queue<std::weak_ptr<TestCase>> m_startQueue;
    QTimer m_startTimer;
    int m_speed = 1;
    int m_interval = 1000;
    QDir m_testcaseBaseDir;
};
