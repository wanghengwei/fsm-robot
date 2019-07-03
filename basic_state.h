#pragma once
#include <QtCore/QState>
#include <QtCore/QTimer>

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

    // 是否输出特定操作的log
    virtual bool printLog() const { return false; }
private:
    void onEntry(QEvent* e) override;
    void onExit(QEvent* e) override;
private:
    QTimer m_timer;
    int m_timeout_ms = 0;
};
