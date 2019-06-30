#pragma once
#include <map>
#include <memory>
#include <QtCore/QString>

class TestCase;

class TestCaseManager {
public:
    void start();
    
    void create(QString account, QString caseName);
private:
    std::map<QString, std::unique_ptr<TestCase>> m_testcases;
};
