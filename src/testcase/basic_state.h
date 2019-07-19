#pragma once
#include <QtCore/QState>
#include <QtCore/QTimer>
#include <map>
#include <string>
#include "testcase.h"

// class Robot;

/** 
 * @brief 表示机器人所处状态
 * 
 * 与最一般的状态不同，这里赋予了更多的意义：
 * 1. 有超时设置。如果设置超时不小于0，那么当指定时间到了后会触发其ev_timeout事件
 * 2. 这里的状态一般是代表做了某个行为然后等待其响应的这一段时间，或者做了某个行为然后等待下一个行为的等待时间
 * 
 * 子类可以重写两个函数：
 * 1. perform 这个函数会在进入状态时执行，一般在这里做点什么，比如发送个消息
 * 2. clean 这个函数是退出状态时清理一些东西的，比如在perform里连接了signal和slot，就需要在clean里将它们断开
*/
class BasicState : public QState {
    Q_OBJECT
public:
    BasicState(QState* parent = nullptr);

    void setTimeout(int ms) { m_timeout_ms = ms; }
    int timeout() const { return m_timeout_ms; }

Q_SIGNALS:
    void ev_timeout();

protected:
    virtual void perform() {}
    virtual void clean() {}

    virtual QString label() const { return this->objectName(); }

    QTimer* timer() { return &m_timer; }

    TestCase& testcase();
    TestCase const& testcase() const;

    const std::string& id() const;
    BasicRobot& robot();

    // 是否输出特定操作的log
    // virtual bool printLog() const { return false; }

    void writeBeginLog(const std::map<std::string, std::any>& args = {});

    void writeEndLogOK(const std::map<std::string, std::string>& results = {});

    void writeEndLogFailed(const std::string& errmsg, const std::map<std::string, std::string>& info = {});
    void writeEndLogFailed(int ec, const std::map<std::string, std::string>& info = {});

private:
    void onEntry(QEvent* e) override;
    void onExit(QEvent* e) override;
private:
    QTimer m_timer;
    int m_timeout_ms = -1;
};

/**
 * @brief 用于在自定义状态的perform函数中获取data
 * 
 * 只能用在自定义的状态中。如果key不存在，那么会打印错误信息然后崩掉 
 * */ 
#define GET_DATA_OR_DIE(key, value) \
    if (!this->testcase().getData(key, value)) { \
        loggers::TESTCASE().critical(R"([{}] [{}:{}] cannot get data by key {})", testcase().id(), __FILE__, __LINE__, key); abort(); }

#define GET_CONFIG_OR_DIE(key, value) \
    if (!this->testcase().getConfig(key, value)) { \
        loggers::TESTCASE().critical(R"([{}] [{}:{}] cannot get config by key {})", testcase().id(), __FILE__, __LINE__, key); abort(); }
