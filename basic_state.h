#pragma once
#include <QtCore/QState>
#include <QtCore/QTimer>
#include <map>
#include <string>
#include "testcase.h"

// class Robot;

class BasicState : public QState {
    Q_OBJECT
public:
    BasicState(QState* parent = nullptr);

    void setTimeout(int ms) { m_timeout_ms = ms; }
    int timeout() const { return m_timeout_ms; }

signals:
    void ev_timeout();

protected:
    virtual void perform(std::map<std::string, std::string>& info) = 0;

    virtual QString label() const { return this->objectName(); }

    QTimer* timer() { return &m_timer; }

    TestCase& testcase();
    TestCase const& testcase() const;

    const std::string& id() const;
    BasicRobot& robot();

    // 是否输出特定操作的log
    virtual bool printLog() const { return false; }

    // template<typename T>
    // bool __attribute_warn_unused_result__ getData(const QString& key, T& value) {
    //     auto tc = qobject_cast<TestCase*>(this->machine());
    //     if (!tc) {
    //         return false;
    //     }

    //     return tc->getData(key, value);
    // }
private:
    void onEntry(QEvent* e) override;
    void onExit(QEvent* e) override;
private:
    QTimer m_timer;
    int m_timeout_ms = 0;
};

/**
 * @brief 用于在自定义状态的perform函数中获取data
 * 
 * 只能用在自定义的状态中。如果key不存在，那么会打印错误信息然后崩掉 
 * */ 
#define GET_DATA_OR_DIE(key, value) \
    if (!this->testcase().getData(key, value)) { \
        loggers::TESTCASE().critical(R"([{}] [{}:{}] cannot get data by key {})", testcase().id(), __FILE__, __LINE__, key); abort(); }
