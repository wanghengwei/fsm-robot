#pragma once
#include <QtCore/QState>
#include <QtCore/QTimer>
#include "testcase.h"

class Robot;

class BasicState : public QState {
    Q_OBJECT
public:
    BasicState(QState* parent = nullptr);

    void setTimeout(int ms) { m_timeout_ms = ms; }
    int timeout() const { return m_timeout_ms; }

signals:
    void ev_timeout();

protected:
    virtual void perform() = 0;

    virtual QString label() const { return this->objectName(); }

    QTimer* timer() { return &m_timer; }

    TestCase& testcase();

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
