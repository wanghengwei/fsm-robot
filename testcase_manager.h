#pragma once
#include <map>
#include <memory>
#include <queue>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <pugixml.hpp>
#include <nlohmann/json.hpp>

class TestCase;

/** 
 * @brief 管理TestCase
*/
class TestCaseManager : public QObject {
    Q_OBJECT
public:
    TestCaseManager();

    /** 
     * @brief 设置用例的执行速率，即每秒最多进入执行状态的用例数量
    */
    void setSpeedRate(int countPerSecond) { m_speed = countPerSecond; }

    /** 
     * @brief 设置用例搜索基础目录
    */
    void setTestCaseDir(QStringView dir) { m_testcaseBaseDir = dir.toString(); }

    /** 
     * @brief 加载用户数据
    */
    void loadUserData(QString const& dir);

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
    nlohmann::json m_userData;
};
