#include "testcase_manager.h"
#include "testcase.h"
#include <pugixml.hpp>
#include "state_factory.h"
#include <memory>
#include "basic_state.h"
#include "idle.h"
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QFinalState>
#include <logger.h>
#include <fstream>
#include <net_base/basic_robot.h>
#include "chart_parser.h"

TestCaseManager::TestCaseManager() {
    QObject::connect(&m_startTimer, &QTimer::timeout, this, &TestCaseManager::startSome);
}

void TestCaseManager::start() {
    for (auto& kv : m_testcases) {
        std::weak_ptr<TestCase> wp{kv.second};
        m_startQueue.emplace(std::move(wp));
    }

    if (m_speed < 10) {
        m_interval = 1000;
    } else if (m_speed < 60) {
        m_interval = 300;
    } else {
        m_interval = 100;
    }

    m_startTimer.start(m_interval);
}

void TestCaseManager::startSome() {
    int n = m_speed * m_interval / 1000;
    int k = 0;
    while (k < n && !m_startQueue.empty()) {
        ++k;
        auto p = m_startQueue.front().lock();
        m_startQueue.pop();
        if (!p) {
            continue;
        }
        p->start();
    }
}

bool TestCaseManager::create(const std::string& id, const QString& caseid) {
    // 创建一个用例对象
    Q_ASSERT(m_xdocManager);
    TestCase* testcase = parseChart(caseid, m_xdocManager);
    std::shared_ptr<TestCase> ptc(testcase);

    // 初始化数据
    // testcase->setObjectName("ROOT");
    testcase->setId(id);
    testcase->setConfig(m_userData);
    Q_ASSERT_X(m_connectionFactory, __PRETTY_FUNCTION__, "forgot set connectionFactory?");
    testcase->setConnectionFactory(m_connectionFactory);

    std::weak_ptr<TestCase> wp{ptc};
    QObject::connect(testcase, &QStateMachine::finished, this, [=]() {
        this->m_startQueue.push(wp);
    });

    m_testcases[id] = std::move(ptc);
    return true;
}

void TestCaseManager::createMany(const std::string& first, int count, const QString& caseName) {
    loggers::TESTCASE_MANAGER().info("create testcases: type=sequence, name={}, first={}, count={}", caseName, first, count);

    bool ok = true;
    auto lfirst = QString::fromStdString(first).toLongLong(&ok);
    if (!ok) {
        // 转换失败
        loggers::TESTCASE_MANAGER().error("first id is not number: {}", first);
        return;
    }

    for (int i = 0; i < count; i++) {
        std::string id = std::to_string(lfirst + i);
        this->create(id, caseName);
    }

    loggers::TESTCASE_MANAGER().info("create testcases OK");
}

void TestCaseManager::loadUserData(QString const& d) {
    QDir dir;
    bool ok = dir.cd(d);
    if (!ok) {
        loggers::TESTCASE_MANAGER().error("cannot find userdata dir: {}", d);
        return;
    }

    // 目前感觉不需要有子目录

    // 最终合并成一个josn
    nlohmann::json merged = nlohmann::json::object();

    auto dfiles = dir.entryList(QStringList{} << "*.json");
    for (auto& df : dfiles) {
        auto p = dir.filePath(df);
        std::ifstream ins{p.toStdString()};
        if (!ins) {
            loggers::TESTCASE_MANAGER().error("cannot read userdata file: {}", df);
            return;
        }

        nlohmann::json j;
        try {
            ins >> j;
        } catch (std::exception& ex) {
            loggers::TESTCASE_MANAGER().error("cannot parse json file {}: what={}", df, ex.what());
            return;
        }

        merged.merge_patch(j);
    }

    loggers::TESTCASE_MANAGER().trace("merged userdata: {}", merged.dump());

    this->m_userData = std::move(merged);
}
